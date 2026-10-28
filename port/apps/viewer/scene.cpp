#include "scene.hpp"

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <map>
#include <set>
#include <memory>

#include "formats/chunk.hpp"
#include "formats/geometry.hpp"
#include "formats/scenery.hpp"
#include "formats/texture_pack.hpp"

namespace viewer {

namespace {

bool load_file(const std::filesystem::path& p, std::vector<uint8_t>& out) {
    std::ifstream f(p, std::ios::binary);
    if (!f) return false;
    out.assign(std::istreambuf_iterator<char>(f), {});
    if (out.size() >= 4 && !std::memcmp(out.data(), "JDLZ", 4)) out = nfsu::unpack_lzc(out.data(), out.size());
    return true;
}

AlphaMode alpha_mode(const nfsu::TextureInfoDisk& ti) {
    if (ti.alpha_blend_type == 0) return ti.alpha_usage_type == 1 ? AlphaMode::Test : AlphaMode::Opaque;
    return ti.alpha_blend_type == 2 ? AlphaMode::Additive : AlphaMode::Blend;
}

struct TexRef {
    const nfsu::TexturePackView* pack;
    size_t index;
};

}  // namespace

std::string list_tracks(nfsu::DataPaths& paths, std::vector<nfsu::TrackInfo>& out) {
    auto p = paths.resolve("GLOBAL\\GLOBALB.BUN");
    if (!p) return "GLOBAL\\GLOBALB.BUN fehlt";
    std::vector<uint8_t> buf;
    if (!load_file(*p, buf)) return "GLOBALB.BUN nicht lesbar";
    return nfsu::parse_track_infos(buf.data(), buf.size(), out);
}

std::string load_scene(nfsu::DataPaths& paths, int number, Scene& out, bool all_sections) {
    std::vector<nfsu::TrackInfo> tracks;
    std::string err = list_tracks(paths, tracks);
    if (!err.empty()) return err;
    auto it = std::find_if(tracks.begin(), tracks.end(), [&](const nfsu::TrackInfo& t) { return t.number == number; });
    if (it == tracks.end()) return "Strecke " + std::to_string(number) + " nicht in TrackInfos";
    out.track = *it;
    nfsu::TrackFiles tf = nfsu::track_files(out.track);

    // Puffer bleiben bis zum Ende leben: TexturePackView zeigt hinein.
    std::vector<std::unique_ptr<std::vector<uint8_t>>> bufs;
    auto open = [&](const std::string& game_path, bool required) -> const std::vector<uint8_t>* {
        auto p = paths.resolve(game_path);
        auto b = std::make_unique<std::vector<uint8_t>>();
        if (!p || !load_file(*p, *b)) {
            if (required) err = game_path + " fehlt";
            return nullptr;
        }
        if (b->empty()) return nullptr;  // z. B. STREAML1RA_1001.BUN hat 0 Byte
        out.files.push_back(game_path);
        bufs.push_back(std::move(b));
        return bufs.back().get();
    };

    const std::vector<uint8_t>* stream = open(tf.stream, true);
    if (!stream) return err;
    const std::vector<uint8_t>* stream_track = open(tf.stream_track, false);
    const std::vector<uint8_t>* tex_track = open(tf.tex_track, false);
    const std::vector<uint8_t>* global = open("GLOBAL\\GLOBALB.BUN", false);
    const std::vector<uint8_t>* ingame = open("GLOBAL\\INGAMEB.BUN", false);  // SFX_LIGHT_BEAM*, GRAY_MID_A, TRACK_BARRIER_FRAME

    // Geometrie: alle Solids des Regionspakets, Zugriff über den Namenshash (wie eModel::Init).
    std::vector<nfsu::GeomSolid> solids;
    err = nfsu::parse_geometry(stream->data(), stream->size(), solids, &out.stats.solids_skipped);
    if (!err.empty()) return tf.stream + ": " + err;
    out.stats.solids = solids.size();
    std::unordered_map<uint32_t, const nfsu::GeomSolid*> by_hash;
    for (const auto& s : solids) by_hash.emplace(s.key_hash, &s);

    std::vector<nfsu::ScenerySection> scen;
    err = nfsu::parse_scenery(stream->data(), stream->size(), scen);
    if (!err.empty()) return tf.stream + ": " + err;

    // Texturen: streckeneigene Pakete zuerst, dann Region, dann Global.
    std::vector<nfsu::TexturePackView> packs;
    for (const auto* b : {stream_track, tex_track, stream, global, ingame}) {
        if (!b) continue;
        err = nfsu::find_texture_packs(b->data(), b->size(), packs);
        if (!err.empty()) return "Texturpaket: " + err;
    }
    std::unordered_map<uint32_t, TexRef> tex_by_hash;
    for (const auto& p : packs)
        for (size_t k = 0; k < p.num_textures; ++k) tex_by_hash.emplace(p.infos[k].name_hash, TexRef{&p, k});

    std::unordered_map<uint32_t, int> tex_index;  // Hash -> Scene::textures
    std::vector<uint8_t> scratch;
    auto texture_for = [&](uint32_t hash) -> int {
        if (hash == 0) return -1;
        auto f = tex_index.find(hash);
        if (f != tex_index.end()) return f->second;
        int idx = -1;
        auto r = tex_by_hash.find(hash);
        if (r != tex_by_hash.end()) {
            const auto& p = *r->second.pack;
            size_t k = r->second.index;
            nfsu::TextureBytes tb;
            Texture t;
            if (nfsu::texture_bytes(p, k, scratch, tb).empty() &&
                nfsu::decode_texture_rgba(p.infos[k], p.plats[k].d3d_format, tb, t.rgba).empty()) {
                t.width = p.infos[k].width;
                t.height = p.infos[k].height;
                t.alpha = alpha_mode(p.infos[k]);
                t.name.assign(p.infos[k].debug_name, strnlen(p.infos[k].debug_name, sizeof p.infos[k].debug_name));
                t.usage = uint8_t(p.infos[k].alpha_usage_type);
                t.blend = uint8_t(p.infos[k].alpha_blend_type);
                idx = int(out.textures.size());
                out.textures.push_back(std::move(t));
            }
        }
        if (idx < 0) ++out.stats.textures_missing;
        tex_index.emplace(hash, idx);
        return idx;
    };

    std::set<int> wanted;
    if (!all_sections) {
        const std::vector<uint8_t>* tb = open(tf.trackb, false);
        nfsu::TrackRoute route;
        if (tb && nfsu::parse_track_route(tb->data(), tb->size(), route).empty()) { wanted = route.wanted(); out.route_sections.assign(route.sections.begin(), route.sections.end()); }
    }
    out.stats.sections_filtered = wanted.size();

    for (const auto& sec : scen) {
        // Panorama-Abschnitte (Buchstabe Z, 26xx: Wasser, Horizont) stehen in keiner Sichtbarkeitsliste.
        if (!wanted.empty() && !wanted.count(sec.number) && sec.number / 100 != 26) continue;
        Section s;
        s.number = sec.number;
        s.name = nfsu::scenery_section_name(sec.number);
        std::map<int, std::vector<uint32_t>> by_tex;  // Textur -> Indizes (Weltvertices)
        for (const auto& inst : sec.instances) {
            ++out.stats.instances;
            const nfsu::SceneryInfoDisk& info = sec.infos[size_t(inst.scenery_info_number)];
            const nfsu::GeomSolid* solid = nullptr;
            for (int lod = 0; lod < 4 && !solid; ++lod) {
                if (!info.name_hash[lod]) continue;
                auto f = by_hash.find(info.name_hash[lod]);
                if (f != by_hash.end()) solid = f->second;
            }
            if (!solid) { ++out.stats.instances_missing; continue; }
            float m[16];
            nfsu::scenery_instance_matrix(inst, m);
            uint32_t base = uint32_t(out.vertices.size());
            for (const auto& gv : solid->vertices) {
                Vertex v;
                v.x = gv.px * m[0] + gv.py * m[4] + gv.pz * m[8] + m[12];
                v.y = gv.px * m[1] + gv.py * m[5] + gv.pz * m[9] + m[13];
                v.z = gv.px * m[2] + gv.py * m[6] + gv.pz * m[10] + m[14];
                v.a = uint8_t(gv.color >> 24);
                v.r = uint8_t(gv.color >> 16);
                v.g = uint8_t(gv.color >> 8);
                v.b = uint8_t(gv.color);
                v.u = gv.u;
                v.v = gv.v;
                const float* pv = &v.x;
                for (int c = 0; c < 3; ++c) {
                    s.bmin[c] = std::min(s.bmin[c], pv[c]);
                    s.bmax[c] = std::max(s.bmax[c], pv[c]);
                }
                out.vertices.push_back(v);
            }
            std::vector<nfsu::GeomBatch> batches = solid->batches;
            if (batches.empty()) batches.push_back({0, uint32_t(solid->indices.size()), solid->texture_hash});
            for (const auto& b : batches) {
                ++out.stats.texture_refs;
                auto& dst = by_tex[texture_for(b.texture_hash)];
                for (uint32_t i = b.start; i < b.start + b.count && i < solid->indices.size(); ++i)
                    dst.push_back(base + solid->indices[i]);
            }
        }
        for (auto& [tex, idx] : by_tex) {
            Batch b;
            b.first_index = uint32_t(out.indices.size());
            b.index_count = uint32_t(idx.size());
            b.texture = tex;
            out.indices.insert(out.indices.end(), idx.begin(), idx.end());
            s.batches.push_back(b);
        }
        if (!s.batches.empty()) out.sections.push_back(std::move(s));
    }
    std::sort(out.sections.begin(), out.sections.end(), [](const Section& a, const Section& b) { return a.number < b.number; });
    return "";
}

}  // namespace viewer
