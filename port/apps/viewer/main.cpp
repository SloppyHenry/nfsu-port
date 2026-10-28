// Strecken-Viewer (WP-18): eine Strecke aus den Originaldateien, gezeichnet mit der MSL-Neuschreibung
// von fx/world.efx über SDL_GPU (Metal), freie Kamera.
//
// Tasten: WASD fahren, Q/E runter/hoch, Umschalt schneller, linke Maustaste fängt die Maus (Blick),
// Tab/Umschalt+Tab Abschnitt wählen, Leertaste Abschnitt ein/aus, O nur diesen Abschnitt, R alle an,
// B Blend-Durchgang ein/aus, P Bildschirmfoto, Esc Maus freigeben bzw. beenden.
#include <SDL3/SDL.h>

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

#include "core/paths.hpp"
#include "formats/png.hpp"
#include "scene.hpp"

namespace {

// Spaltenvektor-Mathematik, Zeilen im Speicher hintereinander (a[r*4+c]). So abgelegt entspricht die
// Matrix im MSL-Puffer genau der spaltenweise abgelegten D3D-Matrix, die world_vs mit v * M erwartet.
struct Mat4 {
    float a[16];
};
Mat4 mul(const Mat4& x, const Mat4& y) {
    Mat4 r{};
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j) {
            float s = 0;
            for (int k = 0; k < 4; ++k) s += x.a[i * 4 + k] * y.a[k * 4 + j];
            r.a[i * 4 + j] = s;
        }
    return r;
}

struct Vec3 {
    float x, y, z;
};
Vec3 operator+(Vec3 a, Vec3 b) { return {a.x + b.x, a.y + b.y, a.z + b.z}; }
Vec3 operator*(Vec3 a, float s) { return {a.x * s, a.y * s, a.z * s}; }
Vec3 cross(Vec3 a, Vec3 b) { return {a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x}; }
float dot(Vec3 a, Vec3 b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
Vec3 normalize(Vec3 a) { return a * (1.0f / std::sqrt(dot(a, a))); }

// Die Spielwelt ist Z-oben (Positionen der Instanzen, Himmelskuppel).
struct Camera {
    Vec3 pos{0, 0, 0};
    float yaw = 0, pitch = -0.3f;  // Bogenmaß
    Vec3 forward() const { return {std::cos(pitch) * std::cos(yaw), std::cos(pitch) * std::sin(yaw), std::sin(pitch)}; }
    Mat4 view() const {
        Vec3 f = forward(), r = normalize(cross(f, {0, 0, 1})), u = cross(r, f);
        return {{r.x, r.y, r.z, -dot(r, pos), u.x, u.y, u.z, -dot(u, pos), -f.x, -f.y, -f.z, dot(f, pos), 0, 0, 0, 1}};
    }
};

// Rechtshändig, Tiefe 0..1 (Metal).
Mat4 perspective(float fovy, float aspect, float n, float f) {
    float t = 1.0f / std::tan(fovy * 0.5f);
    return {{t / aspect, 0, 0, 0, 0, t, 0, 0, 0, 0, f / (n - f), n * f / (n - f), 0, 0, -1, 0}};
}

// Achsenparalleler Quader gegen die sechs Ebenen der Clipmatrix.
bool box_visible(const Mat4& m, const float* bmin, const float* bmax) {
    const float* r0 = &m.a[0];
    const float* r1 = &m.a[4];
    const float* r2 = &m.a[8];
    const float* r3 = &m.a[12];
    float planes[6][4];
    for (int i = 0; i < 4; ++i) {
        planes[0][i] = r3[i] + r0[i];
        planes[1][i] = r3[i] - r0[i];
        planes[2][i] = r3[i] + r1[i];
        planes[3][i] = r3[i] - r1[i];
        planes[4][i] = r2[i];
        planes[5][i] = r3[i] - r2[i];
    }
    for (auto& p : planes) {
        float x = p[0] >= 0 ? bmax[0] : bmin[0];
        float y = p[1] >= 0 ? bmax[1] : bmin[1];
        float z = p[2] >= 0 ? bmax[2] : bmin[2];
        if (p[0] * x + p[1] * y + p[2] * z + p[3] < 0) return false;
    }
    return true;
}

struct WorldUniforms {
    float wvp[16];
    float texture_offset[4];
};

struct Gpu {
    SDL_GPUDevice* dev = nullptr;
    SDL_Window* win = nullptr;
    SDL_GPUGraphicsPipeline* pipe_opaque = nullptr;
    SDL_GPUGraphicsPipeline* pipe_atest = nullptr;
    SDL_GPUGraphicsPipeline* pipe_blend = nullptr;
    SDL_GPUGraphicsPipeline* pipe_add = nullptr;
    SDL_GPUBuffer* vbuf = nullptr;
    SDL_GPUBuffer* ibuf = nullptr;
    SDL_GPUSampler* sampler = nullptr;
    std::vector<SDL_GPUTexture*> textures;
    SDL_GPUTexture* white = nullptr;
    SDL_GPUTexture* color = nullptr;
    SDL_GPUTexture* depth = nullptr;
    uint32_t width = 0, height = 0;
};

constexpr SDL_GPUTextureFormat kColorFormat = SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM;
constexpr SDL_GPUTextureFormat kDepthFormat = SDL_GPU_TEXTUREFORMAT_D32_FLOAT;

bool fail(const char* what) {
    std::fprintf(stderr, "%s: %s\n", what, SDL_GetError());
    return false;
}

// Lädt Daten über einen Transferpuffer in Stücken von höchstens 64 MiB hoch.
bool upload_buffer(SDL_GPUDevice* dev, SDL_GPUBuffer* dst, const void* data, size_t size) {
    const size_t kChunk = 64u << 20;
    SDL_GPUTransferBufferCreateInfo ti{};
    ti.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD;
    ti.size = uint32_t(std::min(size, kChunk));
    SDL_GPUTransferBuffer* tb = SDL_CreateGPUTransferBuffer(dev, &ti);
    if (!tb) return fail("SDL_CreateGPUTransferBuffer");
    for (size_t off = 0; off < size; off += kChunk) {
        size_t n = std::min(kChunk, size - off);
        void* map = SDL_MapGPUTransferBuffer(dev, tb, true);
        std::memcpy(map, static_cast<const uint8_t*>(data) + off, n);
        SDL_UnmapGPUTransferBuffer(dev, tb);
        SDL_GPUCommandBuffer* cmd = SDL_AcquireGPUCommandBuffer(dev);
        SDL_GPUCopyPass* cp = SDL_BeginGPUCopyPass(cmd);
        SDL_GPUTransferBufferLocation src{tb, 0};
        SDL_GPUBufferRegion reg{dst, uint32_t(off), uint32_t(n)};
        SDL_UploadToGPUBuffer(cp, &src, &reg, true);
        SDL_EndGPUCopyPass(cp);
        SDL_GPUFence* fence = SDL_SubmitGPUCommandBufferAndAcquireFence(cmd);
        SDL_WaitForGPUFences(dev, true, &fence, 1);
        SDL_ReleaseGPUFence(dev, fence);
    }
    SDL_ReleaseGPUTransferBuffer(dev, tb);
    return true;
}

SDL_GPUTexture* make_texture(SDL_GPUDevice* dev, int w, int h, const uint8_t* rgba) {
    int levels = 1;
    while ((std::max(w, h) >> levels) > 0) ++levels;
    SDL_GPUTextureCreateInfo ci{};
    ci.type = SDL_GPU_TEXTURETYPE_2D;
    ci.format = kColorFormat;
    ci.usage = SDL_GPU_TEXTUREUSAGE_SAMPLER | SDL_GPU_TEXTUREUSAGE_COLOR_TARGET;  // COLOR_TARGET für Mipmaps
    ci.width = uint32_t(w);
    ci.height = uint32_t(h);
    ci.layer_count_or_depth = 1;
    ci.num_levels = uint32_t(levels);
    SDL_GPUTexture* t = SDL_CreateGPUTexture(dev, &ci);
    if (!t) return nullptr;
    SDL_GPUTransferBufferCreateInfo ti{};
    ti.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD;
    ti.size = uint32_t(w * h * 4);
    SDL_GPUTransferBuffer* tb = SDL_CreateGPUTransferBuffer(dev, &ti);
    std::memcpy(SDL_MapGPUTransferBuffer(dev, tb, false), rgba, ti.size);
    SDL_UnmapGPUTransferBuffer(dev, tb);
    SDL_GPUCommandBuffer* cmd = SDL_AcquireGPUCommandBuffer(dev);
    SDL_GPUCopyPass* cp = SDL_BeginGPUCopyPass(cmd);
    SDL_GPUTextureTransferInfo src{};
    src.transfer_buffer = tb;
    SDL_GPUTextureRegion reg{};
    reg.texture = t;
    reg.w = uint32_t(w);
    reg.h = uint32_t(h);
    reg.d = 1;
    SDL_UploadToGPUTexture(cp, &src, &reg, false);
    SDL_EndGPUCopyPass(cp);
    if (levels > 1) SDL_GenerateMipmapsForGPUTexture(cmd, t);
    SDL_SubmitGPUCommandBuffer(cmd);
    SDL_ReleaseGPUTransferBuffer(dev, tb);
    return t;
}

SDL_GPUShader* load_shader(SDL_GPUDevice* dev, const std::vector<uint8_t>& lib, const char* entry, SDL_GPUShaderStage stage,
                           uint32_t samplers, uint32_t uniforms) {
    SDL_GPUShaderCreateInfo ci{};
    ci.code = lib.data();
    ci.code_size = lib.size();
    ci.entrypoint = entry;
    ci.format = SDL_GPU_SHADERFORMAT_METALLIB;
    ci.stage = stage;
    ci.num_samplers = samplers;
    ci.num_uniform_buffers = uniforms;
    SDL_GPUShader* s = SDL_CreateGPUShader(dev, &ci);
    if (!s) fail(entry);
    return s;
}

bool create_pipelines(Gpu& g, const std::vector<uint8_t>& lib) {
    SDL_GPUShader* vs = load_shader(g.dev, lib, "world_vs", SDL_GPU_SHADERSTAGE_VERTEX, 0, 1);
    SDL_GPUShader* fs = load_shader(g.dev, lib, "world_fs", SDL_GPU_SHADERSTAGE_FRAGMENT, 1, 0);
    SDL_GPUShader* fs_at = load_shader(g.dev, lib, "world_fs_atest", SDL_GPU_SHADERSTAGE_FRAGMENT, 1, 1);
    if (!vs || !fs || !fs_at) return false;

    SDL_GPUVertexBufferDescription vb{};
    vb.slot = 0;
    vb.pitch = sizeof(viewer::Vertex);
    vb.input_rate = SDL_GPU_VERTEXINPUTRATE_VERTEX;
    SDL_GPUVertexAttribute attrs[3] = {
        {0, 0, SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3, 0},
        {1, 0, SDL_GPU_VERTEXELEMENTFORMAT_UBYTE4_NORM, 12},
        {2, 0, SDL_GPU_VERTEXELEMENTFORMAT_FLOAT2, 16},
    };
    SDL_GPUColorTargetDescription ct{};
    ct.format = kColorFormat;

    SDL_GPUGraphicsPipelineCreateInfo pi{};
    pi.vertex_shader = vs;
    pi.fragment_shader = fs;
    pi.vertex_input_state.vertex_buffer_descriptions = &vb;
    pi.vertex_input_state.num_vertex_buffers = 1;
    pi.vertex_input_state.vertex_attributes = attrs;
    pi.vertex_input_state.num_vertex_attributes = 3;
    pi.primitive_type = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST;
    pi.rasterizer_state.cull_mode = SDL_GPU_CULLMODE_NONE;  // world.efx: CullMode NONE
    pi.depth_stencil_state.enable_depth_test = true;
    pi.depth_stencil_state.enable_depth_write = true;
    pi.depth_stencil_state.compare_op = SDL_GPU_COMPAREOP_LESS_OR_EQUAL;
    pi.target_info.color_target_descriptions = &ct;
    pi.target_info.num_color_targets = 1;
    pi.target_info.has_depth_stencil_target = true;
    pi.target_info.depth_stencil_format = kDepthFormat;
    g.pipe_opaque = SDL_CreateGPUGraphicsPipeline(g.dev, &pi);

    pi.fragment_shader = fs_at;
    g.pipe_atest = SDL_CreateGPUGraphicsPipeline(g.dev, &pi);

    // Standardblendung des Originals: BaseSrcBlend SRCALPHA, BaseDestBlend INVSRCALPHA (0x40d1d0).
    pi.fragment_shader = fs;
    pi.depth_stencil_state.enable_depth_write = false;
    ct.blend_state.enable_blend = true;
    ct.blend_state.src_color_blendfactor = SDL_GPU_BLENDFACTOR_SRC_ALPHA;
    ct.blend_state.dst_color_blendfactor = SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_ALPHA;
    ct.blend_state.color_blend_op = SDL_GPU_BLENDOP_ADD;
    ct.blend_state.src_alpha_blendfactor = SDL_GPU_BLENDFACTOR_ONE;
    ct.blend_state.dst_alpha_blendfactor = SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_ALPHA;
    ct.blend_state.alpha_blend_op = SDL_GPU_BLENDOP_ADD;
    g.pipe_blend = SDL_CreateGPUGraphicsPipeline(g.dev, &pi);

    ct.blend_state.dst_color_blendfactor = SDL_GPU_BLENDFACTOR_ONE;  // additiv: TEXBLEND_ADDATIVE
    ct.blend_state.dst_alpha_blendfactor = SDL_GPU_BLENDFACTOR_ONE;
    g.pipe_add = SDL_CreateGPUGraphicsPipeline(g.dev, &pi);

    SDL_ReleaseGPUShader(g.dev, vs);
    SDL_ReleaseGPUShader(g.dev, fs);
    SDL_ReleaseGPUShader(g.dev, fs_at);
    if (!g.pipe_opaque || !g.pipe_atest || !g.pipe_blend || !g.pipe_add) return fail("SDL_CreateGPUGraphicsPipeline");

    SDL_GPUSamplerCreateInfo si{};  // world.efx: WRAP/LINEAR
    si.min_filter = si.mag_filter = SDL_GPU_FILTER_LINEAR;
    si.mipmap_mode = SDL_GPU_SAMPLERMIPMAPMODE_LINEAR;
    si.address_mode_u = si.address_mode_v = si.address_mode_w = SDL_GPU_SAMPLERADDRESSMODE_REPEAT;
    si.max_lod = 1000.0f;
    si.enable_anisotropy = true;
    si.max_anisotropy = 8.0f;
    g.sampler = SDL_CreateGPUSampler(g.dev, &si);
    return g.sampler != nullptr;
}

bool ensure_targets(Gpu& g, uint32_t w, uint32_t h) {
    if (g.color && g.width == w && g.height == h) return true;
    if (g.color) SDL_ReleaseGPUTexture(g.dev, g.color);
    if (g.depth) SDL_ReleaseGPUTexture(g.dev, g.depth);
    SDL_GPUTextureCreateInfo ci{};
    ci.type = SDL_GPU_TEXTURETYPE_2D;
    ci.width = w;
    ci.height = h;
    ci.layer_count_or_depth = 1;
    ci.num_levels = 1;
    ci.format = kColorFormat;
    ci.usage = SDL_GPU_TEXTUREUSAGE_COLOR_TARGET | SDL_GPU_TEXTUREUSAGE_SAMPLER;
    g.color = SDL_CreateGPUTexture(g.dev, &ci);
    ci.format = kDepthFormat;
    ci.usage = SDL_GPU_TEXTUREUSAGE_DEPTH_STENCIL_TARGET;
    g.depth = SDL_CreateGPUTexture(g.dev, &ci);
    g.width = w;
    g.height = h;
    return g.color && g.depth;
}

struct DrawOptions {
    bool blend_pass = true;
    float alpha_ref = 0.5f;
};

struct DrawCounts {
    size_t sections = 0, draws = 0, triangles = 0;
};

DrawCounts draw_scene(Gpu& g, SDL_GPUCommandBuffer* cmd, viewer::Scene& scene, const Camera& cam, const DrawOptions& opt) {
    DrawCounts n;
    float aspect = float(g.width) / float(g.height);
    Mat4 vp = mul(perspective(60.0f * float(M_PI) / 180.0f, aspect, 0.5f, 8000.0f), cam.view());
    WorldUniforms u{};
    std::memcpy(u.wvp, vp.a, sizeof u.wvp);  // Welt = Einheit: die Instanzen sind schon gebacken

    SDL_GPUColorTargetInfo ct{};
    ct.texture = g.color;
    ct.clear_color = {0.35f, 0.42f, 0.55f, 1.0f};
    ct.load_op = SDL_GPU_LOADOP_CLEAR;
    ct.store_op = SDL_GPU_STOREOP_STORE;
    SDL_GPUDepthStencilTargetInfo dt{};
    dt.texture = g.depth;
    dt.clear_depth = 1.0f;
    dt.load_op = SDL_GPU_LOADOP_CLEAR;
    dt.store_op = SDL_GPU_STOREOP_DONT_CARE;
    SDL_GPURenderPass* rp = SDL_BeginGPURenderPass(cmd, &ct, 1, &dt);
    SDL_GPUBufferBinding vb{g.vbuf, 0};
    SDL_GPUBufferBinding ib{g.ibuf, 0};

    std::vector<const viewer::Section*> visible;
    for (const auto& s : scene.sections)
        if (s.visible && box_visible(vp, s.bmin, s.bmax)) visible.push_back(&s);
    n.sections = visible.size();

    auto run = [&](SDL_GPUGraphicsPipeline* pipe, viewer::AlphaMode mode, bool alpha_uniform) {
        SDL_BindGPUGraphicsPipeline(rp, pipe);
        SDL_BindGPUVertexBuffers(rp, 0, &vb, 1);
        SDL_BindGPUIndexBuffer(rp, &ib, SDL_GPU_INDEXELEMENTSIZE_32BIT);
        SDL_PushGPUVertexUniformData(cmd, 0, &u, sizeof u);
        if (alpha_uniform) {
            float ref[4] = {opt.alpha_ref, 0, 0, 0};
            SDL_PushGPUFragmentUniformData(cmd, 0, ref, sizeof ref);
        }
        for (const auto* s : visible)
            for (const auto& b : s->batches) {
                viewer::AlphaMode m = b.texture >= 0 ? scene.textures[size_t(b.texture)].alpha : viewer::AlphaMode::Opaque;
                if (m != mode) continue;
                SDL_GPUTextureSamplerBinding tsb{b.texture >= 0 ? g.textures[size_t(b.texture)] : g.white, g.sampler};
                if (!tsb.texture) tsb.texture = g.white;
                SDL_BindGPUFragmentSamplers(rp, 0, &tsb, 1);
                SDL_DrawGPUIndexedPrimitives(rp, b.index_count, 1, b.first_index, 0, 0);
                ++n.draws;
                n.triangles += b.index_count / 3;
            }
    };
    run(g.pipe_opaque, viewer::AlphaMode::Opaque, false);
    run(g.pipe_atest, viewer::AlphaMode::Test, true);
    if (opt.blend_pass) {
        // Abschnitte von hinten nach vorn, innerhalb eines Abschnitts unsortiert.
        Vec3 c = cam.pos;
        std::sort(visible.begin(), visible.end(), [&](const viewer::Section* a, const viewer::Section* b) {
            auto d = [&](const viewer::Section* s) {
                Vec3 m{(s->bmin[0] + s->bmax[0]) * 0.5f - c.x, (s->bmin[1] + s->bmax[1]) * 0.5f - c.y,
                       (s->bmin[2] + s->bmax[2]) * 0.5f - c.z};
                return dot(m, m);
            };
            return d(a) > d(b);
        });
        run(g.pipe_blend, viewer::AlphaMode::Blend, false);
        run(g.pipe_add, viewer::AlphaMode::Additive, false);
    }
    SDL_EndGPURenderPass(rp);
    return n;
}

bool save_screenshot(Gpu& g, viewer::Scene& scene, const Camera& cam, const DrawOptions& opt, const std::string& path) {
    SDL_GPUCommandBuffer* cmd = SDL_AcquireGPUCommandBuffer(g.dev);
    DrawCounts n = draw_scene(g, cmd, scene, cam, opt);
    SDL_GPUTransferBufferCreateInfo ti{};
    ti.usage = SDL_GPU_TRANSFERBUFFERUSAGE_DOWNLOAD;
    ti.size = g.width * g.height * 4;
    SDL_GPUTransferBuffer* tb = SDL_CreateGPUTransferBuffer(g.dev, &ti);
    SDL_GPUCopyPass* cp = SDL_BeginGPUCopyPass(cmd);
    SDL_GPUTextureRegion reg{};
    reg.texture = g.color;
    reg.w = g.width;
    reg.h = g.height;
    reg.d = 1;
    SDL_GPUTextureTransferInfo dst{};
    dst.transfer_buffer = tb;
    SDL_DownloadFromGPUTexture(cp, &reg, &dst);
    SDL_EndGPUCopyPass(cp);
    SDL_GPUFence* fence = SDL_SubmitGPUCommandBufferAndAcquireFence(cmd);
    SDL_WaitForGPUFences(g.dev, true, &fence, 1);
    SDL_ReleaseGPUFence(g.dev, fence);
    std::vector<uint8_t> rgba(ti.size);
    std::memcpy(rgba.data(), SDL_MapGPUTransferBuffer(g.dev, tb, false), ti.size);
    SDL_UnmapGPUTransferBuffer(g.dev, tb);
    SDL_ReleaseGPUTransferBuffer(g.dev, tb);
    for (size_t i = 3; i < rgba.size(); i += 4) rgba[i] = 255;
    auto png = nfsu::encode_png_rgba(rgba.data(), int(g.width), int(g.height));
    std::filesystem::create_directories(std::filesystem::path(path).parent_path());
    std::ofstream(path, std::ios::binary).write(reinterpret_cast<const char*>(png.data()), std::streamsize(png.size()));
    std::printf("Bildschirmfoto: %s (%ux%u, %zu Abschnitte, %zu Draws, %zu Dreiecke)\n", path.c_str(), g.width, g.height,
                n.sections, n.draws, n.triangles);
    return true;
}

// Strahl aus der Bildmitte gegen alle sichtbaren Dreiecke (Möller-Trumbore); gibt den nächsten Treffer aus.
void pick_center(const viewer::Scene& scene, const Camera& cam) {
    Vec3 o = cam.pos, d = cam.forward();
    float best = 1e30f;
    const viewer::Section* bs = nullptr;
    const viewer::Batch* bb = nullptr;
    for (const auto& s : scene.sections) {
        if (!s.visible) continue;
        for (const auto& b : s.batches) {
            for (uint32_t i = 0; i + 2 < b.index_count; i += 3) {
                const viewer::Vertex* v[3];
                for (int k = 0; k < 3; ++k) v[k] = &scene.vertices[scene.indices[b.first_index + i + k]];
                Vec3 p0{v[0]->x, v[0]->y, v[0]->z}, p1{v[1]->x, v[1]->y, v[1]->z}, p2{v[2]->x, v[2]->y, v[2]->z};
                Vec3 e1 = p1 + p0 * -1.0f, e2 = p2 + p0 * -1.0f, h = cross(d, e2);
                float a = dot(e1, h);
                if (std::fabs(a) < 1e-9f) continue;
                float f = 1.0f / a;
                Vec3 sv = o + p0 * -1.0f;
                float u = f * dot(sv, h);
                if (u < 0 || u > 1) continue;
                Vec3 q = cross(sv, e1);
                float w = f * dot(d, q);
                if (w < 0 || u + w > 1) continue;
                float t = f * dot(e2, q);
                if (t > 0.05f && t < best) { best = t; bs = &s; bb = &b; }
            }
        }
    }
    if (!bb) { std::printf("  Treffer: keiner\n"); return; }
    if (bb->texture < 0) { std::printf("  Treffer: %s, ohne Textur, Abstand %.1f\n", bs->name.c_str(), best); return; }
    const auto& t = scene.textures[size_t(bb->texture)];
    std::printf("  Treffer: Abschnitt %s (%d), Textur %s, usage %d blend %d, %s, Abstand %.1f\n", bs->name.c_str(), bs->number,
                t.name.c_str(), t.usage, t.blend,
                t.alpha == viewer::AlphaMode::Opaque ? "deckend" : t.alpha == viewer::AlphaMode::Test ? "Alphatest" : t.alpha == viewer::AlphaMode::Blend ? "Blend" : "additiv", best);
    std::printf("  --cam %.1f %.1f %.1f %.1f %.1f\n", cam.pos.x, cam.pos.y, cam.pos.z, cam.yaw * 180.0 / M_PI, cam.pitch * 180.0 / M_PI);
    std::fflush(stdout);
}

void default_camera(const viewer::Scene& scene, Camera& cam) {
    float lo[3] = {1e30f, 1e30f, 1e30f}, hi[3] = {-1e30f, -1e30f, -1e30f};
    for (const auto& s : scene.sections) {
        if (s.number / 100 == 26) continue;  // Bibliotheksabschnitte liegen verstreut
        for (int c = 0; c < 3; ++c) {
            lo[c] = std::min(lo[c], s.bmin[c]);
            hi[c] = std::max(hi[c], s.bmax[c]);
        }
    }
    if (lo[0] > hi[0]) return;
    // Mit Streckenführung: auf Fahrbahnhöhe im ersten befahrbaren Abschnitt, Blick zum nächsten.
    std::vector<const viewer::Section*> drive;
    for (int n : scene.route_sections)
        for (const auto& s : scene.sections)
            if (s.number == n && n % 100 > 0 && n % 100 < 10 && n < 2000) drive.push_back(&s);
    if (!drive.empty()) {
        auto centre = [](const viewer::Section& s, int c) { return (s.bmin[c] + s.bmax[c]) * 0.5f; };
        const viewer::Section& a = *drive[0];
        const viewer::Section& b = *drive[drive.size() > 1 ? 1 : 0];
        cam.pos = {centre(a, 0), centre(a, 1), a.bmax[2] + 25.0f};
        float dx = centre(b, 0) - cam.pos.x, dy = centre(b, 1) - cam.pos.y;
        cam.yaw = (dx == 0 && dy == 0) ? 0.0f : std::atan2(dy, dx);
        cam.pitch = -0.45f;
        return;
    }
    cam.pos = {(lo[0] + hi[0]) * 0.5f, (lo[1] + hi[1]) * 0.5f, hi[2] + 50.0f};
    cam.yaw = 0.0f;
    cam.pitch = -0.6f;
}

void print_sections(const viewer::Scene& scene) {
    for (const auto& s : scene.sections) {
        size_t tris = 0;
        for (const auto& b : s.batches) tris += b.index_count / 3;
        std::printf("  %-4s %5d  %4zu Batches %8zu Dreiecke  [%.0f %.0f %.0f]..[%.0f %.0f %.0f]\n", s.name.c_str(), s.number,
                    s.batches.size(), tris, s.bmin[0], s.bmin[1], s.bmin[2], s.bmax[0], s.bmax[1], s.bmax[2]);
    }
}

int usage() {
    std::fprintf(stderr,
                 "Aufruf: nfsu-viewer [--data <pfad>] [--track <nummer>] [--list] [--stats] [--sections] [--all-sections]\n"
                 "                    [--size BxH] [--cam x y z gier nick] [--hide A,B,...] [--no-blend]\n"
                 "                    [--shot <datei.png>]   (ohne Fenster rendern und beenden)\n");
    return 2;
}

}  // namespace

int main(int argc, char** argv) {
    const char* data_arg = nullptr;
    int track = 1001;
    std::string find, hide_tex;
    bool list = false, stats_only = false, sections = false, all_sections = false, have_cam = false;
    std::string shot, hide;
    uint32_t width = 1280, height = 720;
    Camera cam;
    DrawOptions opt;
    for (int i = 1; i < argc; ++i) {
        std::string a = argv[i];
        if (a == "--data" && i + 1 < argc) data_arg = argv[++i];
        else if (a == "--track" && i + 1 < argc) track = std::atoi(argv[++i]);
        else if (a == "--list") list = true;
        else if (a == "--stats") stats_only = true;
        else if (a == "--sections") sections = true;
        else if (a == "--all-sections") all_sections = true;
        else if (a == "--find" && i + 1 < argc) find = argv[++i];
        else if (a == "--hide-tex" && i + 1 < argc) hide_tex = argv[++i];
        else if (a == "--no-blend") opt.blend_pass = false;
        else if (a == "--shot" && i + 1 < argc) shot = argv[++i];
        else if (a == "--hide" && i + 1 < argc) hide = argv[++i];
        else if (a == "--size" && i + 1 < argc) {
            if (std::sscanf(argv[++i], "%ux%u", &width, &height) != 2) return usage();
        } else if (a == "--cam" && i + 5 < argc) {
            cam.pos = {float(std::atof(argv[i + 1])), float(std::atof(argv[i + 2])), float(std::atof(argv[i + 3]))};
            cam.yaw = float(std::atof(argv[i + 4]) * M_PI / 180.0);
            cam.pitch = float(std::atof(argv[i + 5]) * M_PI / 180.0);
            have_cam = true;
            i += 5;
        } else return usage();
    }

    nfsu::DataPaths paths(nfsu::find_data_root(data_arg));
    if (list) {
        std::vector<nfsu::TrackInfo> tracks;
        std::string err = viewer::list_tracks(paths, tracks);
        if (!err.empty()) { std::fprintf(stderr, "Fehler: %s\n", err.c_str()); return 1; }
        for (const auto& t : tracks) std::printf("%5d  %-5s %s\n", t.number, t.region.c_str(), t.name.c_str());
        return 0;
    }

    viewer::Scene scene;
    uint64_t t0 = SDL_GetTicks();
    std::string err = viewer::load_scene(paths, track, scene, all_sections);
    if (!err.empty()) { std::fprintf(stderr, "Fehler: %s\n", err.c_str()); return 1; }
    const auto& st = scene.stats;
    std::printf("Strecke %d \"%s\" (%s), geladen in %.1f s\n", scene.track.number, scene.track.name.c_str(),
                scene.track.region.c_str(), double(SDL_GetTicks() - t0) / 1000.0);
    for (const auto& f : scene.files) std::printf("  Datei %s\n", f.c_str());
    size_t n_test = 0, n_blend = 0;
    for (const auto& t : scene.textures) {
        n_test += t.alpha == viewer::AlphaMode::Test;
        n_blend += t.alpha == viewer::AlphaMode::Blend || t.alpha == viewer::AlphaMode::Additive;
    }
    std::printf("  %zu Solids (%zu ohne Format), %zu Abschnitte, %zu Instanzen (%zu ohne Solid)\n", st.solids,
                st.solids_skipped, scene.sections.size(), st.instances, st.instances_missing);
    std::printf("  %zu Texturen (%zu Alphatest, %zu Blend), %zu Hashes ohne Textur\n", scene.textures.size(), n_test, n_blend,
                st.textures_missing);
    std::printf("  %zu Vertices (%.0f MiB), %zu Dreiecke\n", scene.vertices.size(),
                double(scene.vertices.size() * sizeof(viewer::Vertex)) / 1048576.0, scene.indices.size() / 3);
    if (sections) print_sections(scene);
    if (!hide_tex.empty()) {
        std::string l = "," + hide_tex + ",";
        for (auto& s : scene.sections)
            for (auto& b : s.batches)
                if (b.texture >= 0 && l.find("," + scene.textures[size_t(b.texture)].name + ",") != std::string::npos) b.index_count = 0;
    }
    if (!find.empty()) {
        for (const auto& s : scene.sections)
            for (const auto& b : s.batches) {
                if (b.texture < 0) continue;
                const auto& t = scene.textures[size_t(b.texture)];
                if (t.name.find(find) == std::string::npos) continue;
                std::printf("  %-4s %-28s usage %d blend %d %5u Dreiecke  [%.0f %.0f %.0f]..[%.0f %.0f %.0f]\n", s.name.c_str(),
                            t.name.c_str(), t.usage, t.blend, b.index_count / 3, s.bmin[0], s.bmin[1], s.bmin[2], s.bmax[0], s.bmax[1], s.bmax[2]);
            }
        return 0;
    }
    if (stats_only) return 0;

    for (auto& s : scene.sections) {
        std::string list_ = "," + hide + ",";
        if (!hide.empty() && list_.find("," + s.name + ",") != std::string::npos) s.visible = false;
    }
    if (!have_cam) default_camera(scene, cam);

    if (!SDL_Init(SDL_INIT_VIDEO)) return fail("SDL_Init") ? 0 : 1;
    Gpu g;
    g.dev = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_METALLIB, false, nullptr);
    if (!g.dev) return fail("SDL_CreateGPUDevice") ? 0 : 1;
    if (shot.empty()) {
        g.win = SDL_CreateWindow("NFSU Strecken-Viewer", int(width), int(height), SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY);
        if (!g.win || !SDL_ClaimWindowForGPUDevice(g.dev, g.win)) return fail("Fenster") ? 0 : 1;
    }

    std::vector<uint8_t> lib;
    {
        std::ifstream f(NFSU_SHADER_LIB, std::ios::binary);
        lib.assign(std::istreambuf_iterator<char>(f), {});
        if (lib.empty()) { std::fprintf(stderr, "Shaderbibliothek fehlt: %s\n", NFSU_SHADER_LIB); return 1; }
    }
    if (!create_pipelines(g, lib)) return 1;

    SDL_GPUBufferCreateInfo bi{};
    bi.usage = SDL_GPU_BUFFERUSAGE_VERTEX;
    bi.size = uint32_t(scene.vertices.size() * sizeof(viewer::Vertex));
    g.vbuf = SDL_CreateGPUBuffer(g.dev, &bi);
    bi.usage = SDL_GPU_BUFFERUSAGE_INDEX;
    bi.size = uint32_t(scene.indices.size() * sizeof(uint32_t));
    g.ibuf = SDL_CreateGPUBuffer(g.dev, &bi);
    if (!g.vbuf || !g.ibuf) return fail("SDL_CreateGPUBuffer") ? 0 : 1;
    if (!upload_buffer(g.dev, g.vbuf, scene.vertices.data(), scene.vertices.size() * sizeof(viewer::Vertex)) ||
        !upload_buffer(g.dev, g.ibuf, scene.indices.data(), scene.indices.size() * sizeof(uint32_t)))
        return 1;
    const uint8_t white_px[4] = {255, 255, 255, 255};
    g.white = make_texture(g.dev, 1, 1, white_px);
    for (const auto& t : scene.textures) g.textures.push_back(make_texture(g.dev, t.width, t.height, t.rgba.data()));

    if (!shot.empty()) {
        ensure_targets(g, width, height);
        save_screenshot(g, scene, cam, opt, shot);
        SDL_WaitForGPUIdle(g.dev);
        SDL_Quit();
        return 0;
    }

    size_t selected = 0;
    bool running = true, mouse_look = false;
    uint64_t last = SDL_GetTicksNS(), title_at = 0;
    int frames = 0;
    std::string repo_shot = std::string(NFSU_REPO_ROOT) + "/refs/screens/viewer-" + std::to_string(track) + ".png";
    while (running) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) running = false;
            else if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN && e.button.button == SDL_BUTTON_LEFT) {
                mouse_look = true;
                SDL_SetWindowRelativeMouseMode(g.win, true);
            } else if (e.type == SDL_EVENT_MOUSE_MOTION && mouse_look) {
                cam.yaw -= e.motion.xrel * 0.003f;
                cam.pitch = std::clamp(cam.pitch - e.motion.yrel * 0.003f, -1.55f, 1.55f);
            } else if (e.type == SDL_EVENT_KEY_DOWN) {
                SDL_Keycode k = e.key.key;
                bool shift = (e.key.mod & SDL_KMOD_SHIFT) != 0;
                size_t ns = scene.sections.size();
                if (k == SDLK_ESCAPE) {
                    if (mouse_look) { mouse_look = false; SDL_SetWindowRelativeMouseMode(g.win, false); }
                    else running = false;
                } else if (k == SDLK_TAB && ns) selected = shift ? (selected + ns - 1) % ns : (selected + 1) % ns;
                else if (k == SDLK_SPACE && ns) scene.sections[selected].visible = !scene.sections[selected].visible;
                else if (k == SDLK_O) for (size_t i = 0; i < ns; ++i) scene.sections[i].visible = i == selected;
                else if (k == SDLK_R) for (auto& s : scene.sections) s.visible = true;
                else if (k == SDLK_T) pick_center(scene, cam);
                else if (k == SDLK_B) opt.blend_pass = !opt.blend_pass;
                else if (k == SDLK_P) {
                    save_screenshot(g, scene, cam, opt, repo_shot);
                    std::printf("  --cam %.1f %.1f %.1f %.1f %.1f\n", cam.pos.x, cam.pos.y, cam.pos.z,
                                cam.yaw * 180.0 / M_PI, cam.pitch * 180.0 / M_PI);
                }
            }
        }
        uint64_t now = SDL_GetTicksNS();
        float dt = float(double(now - last) / 1e9);
        last = now;
        const bool* keys = SDL_GetKeyboardState(nullptr);
        float speed = (keys[SDL_SCANCODE_LSHIFT] || keys[SDL_SCANCODE_RSHIFT] ? 200.0f : 40.0f) * dt;
        Vec3 f = cam.forward(), r = normalize(cross(f, {0, 0, 1}));
        if (keys[SDL_SCANCODE_W]) cam.pos = cam.pos + f * speed;
        if (keys[SDL_SCANCODE_S]) cam.pos = cam.pos + f * -speed;
        if (keys[SDL_SCANCODE_D]) cam.pos = cam.pos + r * speed;
        if (keys[SDL_SCANCODE_A]) cam.pos = cam.pos + r * -speed;
        if (keys[SDL_SCANCODE_E]) cam.pos.z += speed;
        if (keys[SDL_SCANCODE_Q]) cam.pos.z -= speed;

        SDL_GPUCommandBuffer* cmd = SDL_AcquireGPUCommandBuffer(g.dev);
        SDL_GPUTexture* swap = nullptr;
        uint32_t sw = 0, sh = 0;
        if (!SDL_WaitAndAcquireGPUSwapchainTexture(cmd, g.win, &swap, &sw, &sh) || !swap) {
            SDL_SubmitGPUCommandBuffer(cmd);
            continue;
        }
        ensure_targets(g, sw, sh);
        DrawCounts n = draw_scene(g, cmd, scene, cam, opt);
        SDL_GPUBlitInfo blit{};
        blit.source.texture = g.color;
        blit.source.w = sw;
        blit.source.h = sh;
        blit.destination.texture = swap;
        blit.destination.w = sw;
        blit.destination.h = sh;
        blit.load_op = SDL_GPU_LOADOP_DONT_CARE;
        blit.filter = SDL_GPU_FILTER_NEAREST;
        SDL_BlitGPUTexture(cmd, &blit);
        SDL_SubmitGPUCommandBuffer(cmd);

        ++frames;
        if (now - title_at > 500000000ull) {
            const auto& s = scene.sections.empty() ? viewer::Section{} : scene.sections[selected];
            char title[256];
            std::snprintf(title, sizeof title, "%s | %.0f fps | %zu Abschn. %zu Draws %.1f M Dr. | Wahl %s (%s) | %.0f %.0f %.0f",
                          scene.track.name.c_str(), frames * 1e9 / double(now - title_at), n.sections, n.draws,
                          double(n.triangles) / 1e6, s.name.c_str(), s.visible ? "an" : "aus", cam.pos.x, cam.pos.y, cam.pos.z);
            SDL_SetWindowTitle(g.win, title);
            if (std::getenv("NFSU_FPS")) std::printf("%s\n", title);
            title_at = now;
            frames = 0;
        }
    }
    SDL_WaitForGPUIdle(g.dev);
    SDL_ReleaseWindowFromGPUDevice(g.dev, g.win);
    SDL_DestroyWindow(g.win);
    SDL_DestroyGPUDevice(g.dev);
    SDL_Quit();
    return 0;
}
