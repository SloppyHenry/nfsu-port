#include "formats/track_info.hpp"

#include <cstring>

#include "formats/chunk.hpp"

namespace nfsu {

static std::string fixed_str(const char* s, size_t n) { return std::string(s, strnlen(s, n)); }

// orig: GC 0x8003E034 TrackInfo::LoaderTrackInfo (Tabelle = Chunkdaten, Anzahl = Größe / 0xFC)
std::string parse_track_infos(const uint8_t* buf, size_t len, std::vector<TrackInfo>& out) {
    bool found = false;
    std::string err = walk_chunks(buf, len, [&](const ChunkView& c) {
        if (c.id != 0x34201) return !found;
        found = true;
        for (size_t i = 0; i + sizeof(TrackInfoDisk) <= c.size; i += sizeof(TrackInfoDisk)) {
            TrackInfoDisk d;
            std::memcpy(&d, c.data + i, sizeof d);
            out.push_back({fixed_str(d.name, sizeof d.name), fixed_str(d.region_name, sizeof d.region_name), d.track_number});
        }
        return false;
    });
    if (!err.empty()) return err;
    return found ? "" : "kein Chunk 0x34201";
}

TrackFiles track_files(const TrackInfo& t) {
    std::string n = std::to_string(t.number);
    return {"TRACKS\\STREAM" + t.region + ".BUN", "TRACKS\\STREAM" + t.region + "_" + n + ".BUN",
            "TRACKS\\TEX" + n + "TRACK.BIN", "TRACKS\\TRACKB" + n + ".LZC"};
}

std::set<int> TrackRoute::wanted() const {
    std::set<int> w;
    auto add = [&](int n) {
        w.insert(n);
        if (n % 100 > 0 && n % 100 < 10) w.insert(n + 10);  // LOD-Abschnitt (ScenerySectionLODOffset)
    };
    for (int16_t n : sections) {
        add(n);
        auto f = visible.find(n);
        if (f != visible.end())
            for (int16_t v : f->second) add(v);
    }
    return w;
}

std::string parse_track_route(const uint8_t* buf, size_t len, TrackRoute& out) {
    auto rd16 = [](const uint8_t* p) { int16_t v; std::memcpy(&v, p, 2); return v; };
    auto rd32 = [](const uint8_t* p) { int32_t v; std::memcpy(&v, p, 4); return v; };
    bool found = false;
    std::string err = walk_chunks(buf, len, [&](const ChunkView& c) {
        if (c.id == 0x34153 && c.size >= 0x10) {
            found = true;
            out.track_number = rd32(c.data + 8);
            int n = rd32(c.data + 12);
            for (int i = 0; i < n && 0x10 + size_t(i) * 2 + 2 <= c.size; ++i) out.sections.push_back(rd16(c.data + 0x10 + i * 2));
        } else if (c.id == 0x34152) {
            for (size_t o = 0; o + 148 <= c.size; o += 148) {
                int sec = rd32(c.data + o + 8), n = rd32(c.data + o + 16);
                auto& v = out.visible[sec];
                for (int i = 0; i < n && i < 64; ++i) v.push_back(rd16(c.data + o + 20 + i * 2));
            }
        }
        return c.container();
    });
    if (!err.empty()) return err;
    return found ? "" : "kein Chunk 0x34153";
}

}  // namespace nfsu
