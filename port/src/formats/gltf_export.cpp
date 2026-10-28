#include "formats/gltf_export.hpp"

#include <cstdio>
#include <fstream>
#include <limits>
#include <map>
#include <sstream>

namespace nfsu {

namespace {

void pad4(std::vector<uint8_t>& bin) { while (bin.size() % 4) bin.push_back(0); }

size_t push_floats(std::vector<uint8_t>& bin, const float* f, size_t n) {
    size_t off = bin.size();
    const uint8_t* p = reinterpret_cast<const uint8_t*>(f);
    bin.insert(bin.end(), p, p + n * sizeof(float));
    return off;
}

size_t push_u16(std::vector<uint8_t>& bin, const std::vector<uint16_t>& v) {
    size_t off = bin.size();
    const uint8_t* p = reinterpret_cast<const uint8_t*>(v.data());
    bin.insert(bin.end(), p, p + v.size() * 2);
    return off;
}

}  // namespace

std::string write_gltf(const std::string& path, const std::vector<GeomSolid>& solids,
                       const GltfTextureLookup& texture_png) {
    if (solids.empty()) return "keine Solids";
    std::string bin_path = path;
    if (bin_path.size() > 5 && bin_path.substr(bin_path.size() - 5) == ".gltf")
        bin_path = bin_path.substr(0, bin_path.size() - 5);
    bin_path += ".bin";
    std::string bin_name = bin_path;
    if (auto p = bin_name.find_last_of('/'); p != std::string::npos) bin_name = bin_name.substr(p + 1);

    std::vector<uint8_t> bin;
    struct BufferView { size_t offset, length; int target; };
    std::vector<BufferView> views;
    struct Accessor { int bufferView, componentType, count; std::string type; bool has_minmax = false; float min[4] = {}, max[4] = {}; };
    std::vector<Accessor> accessors;

    // je Textur-Hash höchstens ein Bild/Material (Mehrfachnutzung über Primitive hinweg)
    std::map<uint32_t, int> material_of_hash;
    struct Image { int bufferView; };
    std::vector<Image> images;
    std::vector<int> textures;    // Index -> image
    std::vector<int> materials_image;  // material_index -> image_index, -1 ohne Bild

    auto add_view = [&](size_t off, size_t len, int target) {
        views.push_back({off, len, target});
        return int(views.size()) - 1;
    };

    std::ostringstream prims;
    bool first_prim = true;
    for (const auto& s : solids) {
        if (s.vertices.empty() || s.indices.empty()) continue;
        size_t n = s.vertices.size();
        std::vector<float> pos(n * 3), nrm(n * 3), col(n * 4), uv(n * 2);
        float minp[3] = {std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max()};
        float maxp[3] = {-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max(), -std::numeric_limits<float>::max()};
        for (size_t i = 0; i < n; ++i) {
            const auto& v = s.vertices[i];
            pos[i * 3 + 0] = v.px; pos[i * 3 + 1] = v.py; pos[i * 3 + 2] = v.pz;
            nrm[i * 3 + 0] = v.nx; nrm[i * 3 + 1] = v.ny; nrm[i * 3 + 2] = v.nz;
            uv[i * 2 + 0] = v.u; uv[i * 2 + 1] = v.v;
            col[i * 4 + 0] = float((v.color >> 16) & 0xff) / 255.0f;
            col[i * 4 + 1] = float((v.color >> 8) & 0xff) / 255.0f;
            col[i * 4 + 2] = float(v.color & 0xff) / 255.0f;
            col[i * 4 + 3] = float((v.color >> 24) & 0xff) / 255.0f;
            for (int c = 0; c < 3; ++c) {
                float p = (&v.px)[c];
                minp[c] = std::min(minp[c], p);
                maxp[c] = std::max(maxp[c], p);
            }
        }

        size_t off_pos = push_floats(bin, pos.data(), pos.size()); pad4(bin);
        int bv_pos = add_view(off_pos, pos.size() * 4, 34962);
        size_t off_nrm = push_floats(bin, nrm.data(), nrm.size()); pad4(bin);
        int bv_nrm = add_view(off_nrm, nrm.size() * 4, 34962);
        size_t off_col = push_floats(bin, col.data(), col.size()); pad4(bin);
        int bv_col = add_view(off_col, col.size() * 4, 34962);
        size_t off_uv = push_floats(bin, uv.data(), uv.size()); pad4(bin);
        int bv_uv = add_view(off_uv, uv.size() * 4, 34962);
        size_t off_idx = push_u16(bin, s.indices); pad4(bin);

        Accessor ap{bv_pos, 5126, int(n), "VEC3", true};
        for (int c = 0; c < 3; ++c) { ap.min[c] = minp[c]; ap.max[c] = maxp[c]; }
        int a_pos = int(accessors.size()); accessors.push_back(ap);
        int a_nrm = int(accessors.size()); accessors.push_back({bv_nrm, 5126, int(n), "VEC3"});
        int a_col = int(accessors.size()); accessors.push_back({bv_col, 5126, int(n), "VEC4"});
        int a_uv = int(accessors.size()); accessors.push_back({bv_uv, 5126, int(n), "VEC2"});
        auto bv_idx_all = [&](const GeomBatch& b) {
            return add_view(off_idx + size_t(b.start) * 2, size_t(b.count) * 2, 34963);
        };

        auto material_for = [&](uint32_t hash) {
            auto it = material_of_hash.find(hash);
            if (it != material_of_hash.end()) return it->second;
            int image_idx = -1;
            if (hash != 0 && texture_png) {
                auto png = texture_png(hash);
                if (!png.empty()) {
                    size_t off_img = bin.size();
                    bin.insert(bin.end(), png.begin(), png.end());
                    pad4(bin);
                    int bv_img = add_view(off_img, png.size(), -1);
                    image_idx = int(images.size());
                    images.push_back({bv_img});
                    textures.push_back(image_idx);
                }
            }
            int material = int(materials_image.size());
            materials_image.push_back(image_idx >= 0 ? int(textures.size()) - 1 : -1);
            material_of_hash[hash] = material;
            return material;
        };

        std::vector<GeomBatch> batches = s.batches;
        if (batches.empty()) batches.push_back({0, uint32_t(s.indices.size()), s.texture_hash});
        for (const auto& b : batches) {
            // je Batch eigener Indexbereich (Byte-Offset im gemeinsamen Indexpuffer)
            int a_idx_b = int(accessors.size());
            Accessor ai{bv_idx_all(b), 5123, int(b.count), "SCALAR"};
            accessors.push_back(ai);
            int material = material_for(b.texture_hash);
            if (!first_prim) prims << ",\n";
            first_prim = false;
            prims << "    {\"attributes\":{\"POSITION\":" << a_pos << ",\"NORMAL\":" << a_nrm
                  << ",\"COLOR_0\":" << a_col << ",\"TEXCOORD_0\":" << a_uv << "},"
                  << "\"indices\":" << a_idx_b << ",\"material\":" << material << ",\"mode\":4}";
        }
    }
    if (first_prim) return "keine exportierbaren Solids (alle leer)";

    std::ofstream bf(bin_path, std::ios::binary);
    if (!bf) return "kann " + bin_path + " nicht schreiben";
    bf.write(reinterpret_cast<const char*>(bin.data()), std::streamsize(bin.size()));
    bf.close();

    std::ostringstream j;
    j << "{\n  \"asset\":{\"version\":\"2.0\",\"generator\":\"nfsu-tool mesh (WP-14)\"},\n";
    j << "  \"buffers\":[{\"uri\":\"" << bin_name << "\",\"byteLength\":" << bin.size() << "}],\n";
    j << "  \"bufferViews\":[\n";
    for (size_t i = 0; i < views.size(); ++i) {
        const auto& v = views[i];
        j << "    {\"buffer\":0,\"byteOffset\":" << v.offset << ",\"byteLength\":" << v.length;
        if (v.target > 0) j << ",\"target\":" << v.target;
        j << "}" << (i + 1 < views.size() ? "," : "") << "\n";
    }
    j << "  ],\n  \"accessors\":[\n";
    for (size_t i = 0; i < accessors.size(); ++i) {
        const auto& a = accessors[i];
        j << "    {\"bufferView\":" << a.bufferView << ",\"componentType\":" << a.componentType
          << ",\"count\":" << a.count << ",\"type\":\"" << a.type << "\"";
        if (a.has_minmax) {
            j << ",\"min\":[" << a.min[0] << "," << a.min[1] << "," << a.min[2] << "]"
              << ",\"max\":[" << a.max[0] << "," << a.max[1] << "," << a.max[2] << "]";
        }
        j << "}" << (i + 1 < accessors.size() ? "," : "") << "\n";
    }
    j << "  ],\n";
    if (!images.empty()) {
        j << "  \"images\":[\n";
        for (size_t i = 0; i < images.size(); ++i)
            j << "    {\"bufferView\":" << images[i].bufferView << ",\"mimeType\":\"image/png\"}"
              << (i + 1 < images.size() ? "," : "") << "\n";
        j << "  ],\n  \"textures\":[\n";
        for (size_t i = 0; i < textures.size(); ++i)
            j << "    {\"source\":" << textures[i] << "}" << (i + 1 < textures.size() ? "," : "") << "\n";
        j << "  ],\n";
    }
    j << "  \"materials\":[\n";
    for (size_t i = 0; i < materials_image.size(); ++i) {
        j << "    {\"pbrMetallicRoughness\":{\"baseColorFactor\":[1,1,1,1]";
        if (materials_image[i] >= 0) j << ",\"baseColorTexture\":{\"index\":" << materials_image[i] << "}";
        j << ",\"metallicFactor\":0,\"roughnessFactor\":1}}";
        j << (i + 1 < materials_image.size() ? "," : "") << "\n";
    }
    j << "  ],\n";
    j << "  \"meshes\":[{\"primitives\":[\n" << prims.str() << "\n  ]}],\n";
    j << "  \"nodes\":[{\"mesh\":0}],\n";
    j << "  \"scenes\":[{\"nodes\":[0]}],\n";
    j << "  \"scene\":0\n}\n";

    std::ofstream gf(path);
    if (!gf) return "kann " + path + " nicht schreiben";
    gf << j.str();
    return {};
}

}  // namespace nfsu
