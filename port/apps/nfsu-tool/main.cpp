#include <algorithm>
#include <cstdio>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <vector>
#include <string>

#include "core/paths.hpp"
#include "formats/chunk.hpp"
#include "formats/ea_audio.hpp"
#include "formats/geometry.hpp"
#include "formats/gltf_export.hpp"
#include "formats/png.hpp"
#include "formats/texture_pack.hpp"
#include "platform/video.hpp"

static bool load(const std::string& p, std::vector<uint8_t>& out) {
    std::ifstream f(p, std::ios::binary);
    if (!f) return false;
    out.assign(std::istreambuf_iterator<char>(f), {});
    return true;
}

// .lzc wird entpackt, alles andere roh gelesen.
static bool load_unpacked(const std::string& p, std::vector<uint8_t>& out) {
    std::vector<uint8_t> raw;
    if (!load(p, raw)) return false;
    if (raw.size() >= 4 && !std::memcmp(raw.data(), "JDLZ", 4)) {
        out = nfsu::unpack_lzc(raw.data(), raw.size());
        return !out.empty();
    }
    out = std::move(raw);
    return true;
}

static int usage() {
    std::fprintf(stderr, "Aufruf: nfsu-tool [--data <pfad>] ls <muster>\n"
                 "       nfsu-tool chunks <datei> [--depth N]\n       nfsu-tool unpack <datei.lzc> <aus>\n"
                 "       nfsu-tool snd <datei.ast> [<aus.wav|aus-Ordner>] | snd-md5 <datei.ast>\n       nfsu-tool abk <datei.abk> [<aus-Ordner>]\n       nfsu-tool play <Movies/x.mad> [--frames N]\n       nfsu-tool movie-md5 <Movies/x.mad>\n"
                 "       nfsu-tool efx-decode [<aus-Ordner>]   (fx/*.efx, XOR 0xAA, Standard refs/fx)\n"
                 "       nfsu-tool mesh <GEOMETRY.BIN|Streckenpaket> <aus.gltf> [--textures <TEXTURES.BIN>]\n");
    return 2;
}

static const char* fmt_name(uint32_t f) {
    switch (f) {
    case nfsu::kD3DFmtDXT1: return "DXT1";
    case nfsu::kD3DFmtDXT3: return "DXT3";
    case nfsu::kD3DFmtDXT5: return "DXT5";
    case nfsu::kD3DFmtA8R8G8B8: return "ARGB";
    case nfsu::kD3DFmtP8: return "P8";
    default: return "?";
    }
}

// Kontaktbogen: Zellen von cell×cell Pixeln, Bilder seitengetreu verkleinert, Alpha über Schachbrett.
struct Sheet {
    int cell = 128, cols = 16;
    std::vector<std::vector<uint8_t>> imgs;
    std::vector<std::pair<int, int>> sizes;
    void add(const std::vector<uint8_t>& rgba, int w, int h) { imgs.push_back(rgba); sizes.push_back({w, h}); }
    bool write(const std::string& path) const {
        if (imgs.empty()) return false;
        int c = std::min<int>(cols, int(imgs.size())), r = int((imgs.size() + cols - 1) / cols);
        int W = c * cell, H = r * cell;
        std::vector<uint8_t> out(size_t(W) * H * 4);
        for (int y = 0; y < H; ++y)
            for (int x = 0; x < W; ++x) {
                uint8_t g = ((x / 8 + y / 8) & 1) ? 0x60 : 0x90;
                uint8_t* o = &out[(size_t(y) * W + x) * 4];
                o[0] = o[1] = o[2] = g; o[3] = 255;
            }
        for (size_t k = 0; k < imgs.size(); ++k) {
            auto [w, h] = sizes[k];
            int cw = cell - 2, ch = cell - 2;
            double sc = std::min(double(cw) / w, double(ch) / h);
            int tw = std::max(1, int(w * sc)), th = std::max(1, int(h * sc));
            int ox = int(k % cols) * cell + 1, oy = int(k / cols) * cell + 1;
            for (int y = 0; y < th; ++y)
                for (int x = 0; x < tw; ++x) {
                    const uint8_t* s = &imgs[k][(size_t(y * h / th) * w + size_t(x * w / tw)) * 4];
                    uint8_t* o = &out[(size_t(oy + y) * W + ox + x) * 4];
                    for (int j = 0; j < 3; ++j) o[j] = uint8_t((s[j] * s[3] + o[j] * (255 - s[3])) / 255);
                }
        }
        return nfsu::write_png_rgba(path, out.data(), W, H);
    }
};

int main(int argc, char** argv) {
    const char* data_arg = nullptr;
    int i = 1;
    if (i + 1 < argc && !std::strcmp(argv[i], "--data")) { data_arg = argv[i + 1]; i += 2; }
    if (i >= argc) return usage();

    nfsu::DataPaths paths(nfsu::find_data_root(data_arg));
    std::string cmd = argv[i++];
    if (cmd == "ls" && i < argc) {
        auto hits = paths.glob(argv[i]);
        for (const auto& p : hits) std::printf("%s\n", p.c_str());
        return hits.empty() ? 1 : 0;
    }
    if ((cmd == "play" || cmd == "movie-md5") && i < argc) {
        std::string rel = argv[i++];
        auto abs = paths.resolve(rel);
        if (!abs) { std::fprintf(stderr, "nicht gefunden: %s\n", rel.c_str()); return 1; }
        if (cmd == "play") {
            int frames = (i + 1 < argc && !std::strcmp(argv[i], "--frames")) ? std::atoi(argv[i + 1]) : 0;
            return nfsu::movie_play(abs->string(), frames);
        }
        nfsu::MovieInfo mi; std::vector<std::string> md5; std::string err;
        if (!nfsu::movie_scan(abs->string(), mi, &md5, err)) { std::fprintf(stderr, "%s\n", err.c_str()); return 1; }
        for (size_t k = 0; k < md5.size(); ++k) std::printf("%zu %s\n", k, md5[k].c_str());
        std::fprintf(stderr, "%dx%d %.3f fps, %llu Bilder, Ton %d Hz x%d, %llu Samples\n", mi.width, mi.height, mi.fps,
                     (unsigned long long)mi.video_frames, mi.audio_rate, mi.audio_channels, (unsigned long long)mi.audio_samples);
        return 0;
    }
    if (cmd == "chunks" && i < argc) {
        std::string file = argv[i++];
        int maxd = 99;
        if (i + 1 < argc && !std::strcmp(argv[i], "--depth")) maxd = std::atoi(argv[i + 1]);
        std::vector<uint8_t> buf;
        if (!load_unpacked(file, buf)) { std::fprintf(stderr, "kann %s nicht lesen/entpacken\n", file.c_str()); return 1; }
        std::string err = nfsu::walk_chunks(buf.data(), buf.size(), [&](const nfsu::ChunkView& c) {
            if (c.depth <= maxd)
                std::printf("%*s%08x %s size=%u @%zu\n", c.depth * 2, "", c.id, c.container() ? "C" : "-", c.size, c.offset);
            return c.depth < maxd;
        });
        if (!err.empty()) { std::fprintf(stderr, "Fehler: %s\n", err.c_str()); return 1; }
        return 0;
    }
    if (cmd == "mesh" && i + 1 < argc) {
        std::string file = argv[i++], out = argv[i++], tex_file;
        for (; i < argc; ++i) {
            if (!std::strcmp(argv[i], "--textures") && i + 1 < argc) tex_file = argv[++i];
        }
        std::vector<uint8_t> buf;
        if (!load_unpacked(file, buf)) { std::fprintf(stderr, "kann %s nicht lesen/entpacken\n", file.c_str()); return 1; }
        std::vector<nfsu::GeomSolid> solids;
        size_t skipped = 0;
        std::string err = nfsu::parse_geometry(buf.data(), buf.size(), solids, &skipped);
        if (!err.empty()) { std::fprintf(stderr, "Fehler: %s\n", err.c_str()); return 1; }
        size_t nverts = 0, nidx = 0;
        for (const auto& s : solids) { nverts += s.vertices.size(); nidx += s.indices.size(); }
        std::printf("%zu Solids (%zu uebersprungen, unbekanntes Format), %zu Vertices, %zu Indizes\n",
                    solids.size(), skipped, nverts, nidx);

        std::vector<uint8_t> tex_buf;
        std::vector<nfsu::TexturePackView> packs;
        if (!tex_file.empty()) {
            if (!load_unpacked(tex_file, tex_buf)) { std::fprintf(stderr, "kann %s nicht lesen/entpacken\n", tex_file.c_str()); return 1; }
            err = nfsu::find_texture_packs(tex_buf.data(), tex_buf.size(), packs);
            if (!err.empty()) { std::fprintf(stderr, "Fehler: %s\n", err.c_str()); return 1; }
        }
        nfsu::GltfTextureLookup lookup;
        if (!packs.empty()) {
            lookup = [&packs](uint32_t hash) -> std::vector<uint8_t> {
                std::vector<uint8_t> scratch, rgba;
                for (const auto& p : packs) {
                    for (size_t k = 0; k < p.num_textures; ++k) {
                        if (p.infos[k].name_hash != hash) continue;
                        nfsu::TextureBytes tb;
                        if (!nfsu::texture_bytes(p, k, scratch, tb).empty()) return {};
                        if (!nfsu::decode_texture_rgba(p.infos[k], p.plats[k].d3d_format, tb, rgba).empty()) return {};
                        return nfsu::encode_png_rgba(rgba.data(), p.infos[k].width, p.infos[k].height);
                    }
                }
                return {};
            };
        }
        err = nfsu::write_gltf(out, solids, lookup);
        if (!err.empty()) { std::fprintf(stderr, "Fehler: %s\n", err.c_str()); return 1; }
        std::printf("geschrieben: %s\n", out.c_str());
        return 0;
    }
    if ((cmd == "snd" || cmd == "snd-md5") && i < argc) {
        std::vector<uint8_t> raw;
        if (!load(argv[i], raw)) { std::fprintf(stderr, "nicht lesbar\n"); return 1; }
        std::vector<nfsu::ea::Stream> ss;
        std::string err = nfsu::ea::scan_streams(raw.data(), raw.size(), ss);
        if (!err.empty()) { std::fprintf(stderr, "Fehler: %s\n", err.c_str()); return 1; }
        std::string outdir = (i + 1 < argc) ? argv[i + 1] : "";
        for (size_t k = 0; k < ss.size(); ++k) {
            const auto& s = ss[k];
            std::vector<int16_t> pcm;
            err = nfsu::ea::decode_stream(raw.data(), raw.size(), s, pcm);
            if (!err.empty()) { std::fprintf(stderr, "Strom %zu: %s\n", k, err.c_str()); return 1; }
            size_t frames = pcm.size() / s.hdr.channels;
            if (cmd == "snd-md5") {
                std::fwrite(pcm.data(), 2, pcm.size(), stdout);   // roh nach stdout, fuer md5 / Vergleich
            } else if (outdir.empty()) {
                std::printf("%3zu  @%-10zu %zu B  %u Hz  %uch  %u Bloecke  %zu/%u Samples (%.1f s)%s\n", k, s.offset, s.size,
                            s.hdr.sample_rate, s.hdr.channels, s.blocks, frames, s.hdr.num_samples,
                            double(frames) / s.hdr.sample_rate, frames + 28 > s.hdr.num_samples && frames <= s.hdr.num_samples ? "" : "  !");
            } else {
                auto w = nfsu::ea::make_wav(pcm.data(), frames, s.hdr.channels, s.hdr.sample_rate);
                char name[512]; std::snprintf(name, sizeof name, "%s/%03zu.wav", outdir.c_str(), k);
                std::ofstream(name, std::ios::binary).write(reinterpret_cast<const char*>(w.data()), w.size());
            }
        }
        return 0;
    }
    if (cmd == "abk" && i < argc) {
        std::vector<uint8_t> raw;
        if (!load(argv[i], raw)) { std::fprintf(stderr, "nicht lesbar\n"); return 1; }
        nfsu::ea::Bank bank;
        std::string err = nfsu::ea::parse_abk(raw.data(), raw.size(), bank);
        if (!err.empty()) { std::fprintf(stderr, "Fehler: %s\n", err.c_str()); return 1; }
        std::string outdir = (i + 1 < argc) ? argv[i + 1] : "";
        if (outdir.empty())
            std::printf("ABKC v%x  %u B  Abbild %u B  Flaeche A %u+%u  B %u+%u  Tabellen %u/%u/%u  %zu Sounds\n", bank.version,
                        bank.file_size, bank.header_size, bank.area_off[0], bank.area_size[0], bank.area_off[1],
                        bank.area_size[1], bank.table_count[0], bank.table_count[1], bank.table_count[2], bank.sounds.size());
        int rc = 0;
        for (const auto& s : bank.sounds) {
            std::vector<int16_t> pcm;
            size_t used = 0;
            err = nfsu::ea::decode_bnk_sound(raw.data(), raw.size(), s, pcm, &used);
            if (!err.empty()) { std::fprintf(stderr, "Sound %u: %s\n", s.index, err.c_str()); rc = 1; continue; }
            if (outdir.empty()) {
                double sxy = 0, sxx = 1;
                for (size_t k = 0; k + 1 < pcm.size(); ++k) { sxy += double(pcm[k]) * pcm[k + 1]; sxx += double(pcm[k]) * pcm[k]; }
                std::printf("  A%d #%-3u %-6s %5u Hz %7zu Samples  Daten %zu/%zu B  Schleife %u..%u  Korrelation %.2f\n", s.area, s.index,
                            s.codec2 == 8 ? "PCM16" : "EA-XA", s.hdr.sample_rate, pcm.size(), used, s.data_limit - s.data_offset,
                            s.loop_start, s.loop_end, sxy / sxx);
            } else {
                auto w = nfsu::ea::make_wav(pcm.data(), pcm.size(), 1, s.hdr.sample_rate);
                char name[512]; std::snprintf(name, sizeof name, "%s/%d_%03u.wav", outdir.c_str(), s.area, s.index);
                std::ofstream(name, std::ios::binary).write(reinterpret_cast<const char*>(w.data()), w.size());
            }
        }
        return rc;
    }
    if (cmd == "tex" && i < argc) {
        std::string file = argv[i++], outdir, sheet_path;
        for (; i < argc; ++i) {
            if (!std::strcmp(argv[i], "--sheet") && i + 1 < argc) sheet_path = argv[++i];
            else outdir = argv[i];
        }
        std::vector<uint8_t> buf;
        if (!load_unpacked(file, buf)) { std::fprintf(stderr, "kann %s nicht lesen/entpacken\n", file.c_str()); return 1; }
        std::vector<nfsu::TexturePackView> packs;
        std::string err = nfsu::find_texture_packs(buf.data(), buf.size(), packs);
        if (!err.empty()) { std::fprintf(stderr, "Fehler: %s\n", err.c_str()); return 1; }
        size_t total = 0, ok = 0;
        Sheet sheet;
        std::vector<uint8_t> scratch, rgba;
        std::vector<std::string> used_dirs;
        for (const auto& p : packs) {
            std::printf("Paket %s  %zu Texturen  %s  Daten %zu B\n", p.name().c_str(), p.num_textures,
                        p.streams ? "einzeln JDLZ" : "roh", p.data_size);
            std::string pdir = outdir.empty() ? "" : outdir + "/" + p.name();
            // gleichnamige Pakete in einer Datei: Ordner durchzählen statt überschreiben
            int dup = int(std::count(used_dirs.begin(), used_dirs.end(), pdir));
            used_dirs.push_back(pdir);
            if (dup && !pdir.empty()) pdir += "_" + std::to_string(dup);
            if (!pdir.empty()) std::filesystem::create_directories(pdir);
            for (size_t k = 0; k < p.num_textures; ++k) {
                const auto& t = p.infos[k];
                std::string name(t.debug_name, strnlen(t.debug_name, sizeof t.debug_name));
                uint32_t fmt = p.plats[k].d3d_format;
                ++total;
                nfsu::TextureBytes tb;
                err = nfsu::texture_bytes(p, k, scratch, tb);
                if (err.empty()) err = nfsu::decode_texture_rgba(t, fmt, tb, rgba);
                if (!err.empty()) {
                    std::printf("  FEHLER %-24s %08x %dx%d %s: %s\n", name.c_str(), t.name_hash, t.width, t.height,
                                fmt_name(fmt), err.c_str());
                    continue;
                }
                ++ok;
                if (outdir.empty() && sheet_path.empty())
                    std::printf("  %-24s %08x %4dx%-4d %-4s typ %02x mip %d pal %d\n", name.c_str(), t.name_hash, t.width,
                                t.height, fmt_name(fmt), t.image_compression_type, t.num_mipmap_levels, t.num_palette_entries);
                if (!pdir.empty()) {
                    char fn[64];
                    std::snprintf(fn, sizeof fn, "_%08x.png", t.name_hash);
                    if (!nfsu::write_png_rgba(pdir + "/" + name + fn, rgba.data(), t.width, t.height)) {
                        std::fprintf(stderr, "kann %s nicht schreiben\n", (pdir + "/" + name + fn).c_str());
                        return 1;
                    }
                }
                if (!sheet_path.empty()) sheet.add(rgba, t.width, t.height);
            }
        }
        if (!sheet_path.empty() && !sheet.write(sheet_path)) { std::fprintf(stderr, "Kontaktbogen fehlgeschlagen\n"); return 1; }
        std::printf("Summe: %zu Pakete, %zu Texturen, %zu dekodiert, %zu Fehler\n", packs.size(), total, ok, total - ok);
        return ok == total ? 0 : 1;
    }
    // orig: 0x40d1d0 init_render_devices liest fx\*.efx und XORt jedes Byte mit 0xAA
    if (cmd == "efx-decode") {
        std::filesystem::path outdir = (i < argc) ? argv[i] : (std::filesystem::path(NFSU_DEFAULT_DATA) / ".." / ".." / "refs" / "fx");
        std::filesystem::create_directories(outdir);
        auto hits = paths.glob("fx/*");
        int n = 0;
        for (const auto& p : hits) {
            std::vector<uint8_t> raw;
            if (!load(p, raw)) continue;
            for (auto& b : raw) b ^= 0xAA;
            std::string name = std::filesystem::path(p).filename().string();
            std::ofstream(outdir / name, std::ios::binary).write(reinterpret_cast<const char*>(raw.data()), raw.size());
            std::printf("%s: %zu Bytes\n", name.c_str(), raw.size());
            ++n;
        }
        return n ? 0 : 1;
    }
    if (cmd == "unpack" && i + 1 < argc) {
        std::vector<uint8_t> raw;
        if (!load(argv[i], raw)) return 1;
        auto out = nfsu::unpack_lzc(raw.data(), raw.size());
        if (out.empty()) { std::fprintf(stderr, "kein gueltiges JDLZ\n"); return 1; }
        std::ofstream(argv[i + 1], std::ios::binary).write(reinterpret_cast<const char*>(out.data()), out.size());
        return 0;
    }
    return usage();
}
