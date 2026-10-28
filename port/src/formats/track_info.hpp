// Streckentabelle (Chunk 0x34201 "TrackInfos" in GLOBAL\GLOBALB.BUN) und die Dateien einer Strecke.
// Name TrackInfo aus der GC-Speed.elf (TrackInfo::GetTrackInfo 0x8003DFC0: Eintrag 0xFC Byte,
// TrackNumber als short an +0x6C). Die übrigen Felder sind aus den Daten abgelesen (WP-18),
// der Typ fehlt in symbols/gc_types.h.
#pragma once
#include <cstddef>
#include <cstdint>
#include <map>
#include <set>
#include <string>
#include <vector>

namespace nfsu {

#pragma pack(push, 1)
struct TrackInfoDisk {
    char name[32];              // +0x00 "CT1 - Market St."
    char track_directory[32];   // +0x20 "Location1\Track_1001"
    char region_name[8];        // +0x40 "L1RA"
    char region_directory[32];  // +0x48 "Location1\RegionA"
    int32_t unknown68;          // +0x68
    int16_t track_number;       // +0x6C 1001
    int16_t same_as_track_number;  // +0x6E in allen 31 Einträgen gleich +0x6C
    uint8_t rest[0xfc - 0x70];
};
#pragma pack(pop)
static_assert(sizeof(TrackInfoDisk) == 0xfc, "TrackInfo");

struct TrackInfo {
    std::string name, region;
    int number = 0;
};

// orig: GC 0x8003E034 TrackInfo::LoaderTrackInfo (nur das Ablesen). Leer bei Erfolg.
std::string parse_track_infos(const uint8_t* buf, size_t len, std::vector<TrackInfo>& out);

// Spielpfade einer Strecke in Ladereihenfolge. Formatstrings aus Speed.exe:
// "TRACKS\STREAM%s.BUN", "TRACKS\STREAM%s_%d.BUN", "TRACKS\TEX%dTRACK.BIN", "TRACKS\TRACKB%d.LZC".
struct TrackFiles {
    std::string stream, stream_track, tex_track, trackb;
};
TrackFiles track_files(const TrackInfo& t);

// Streckenführung aus TRACKB<id>.LZC: Abschnitte der Strecke (0x34153, LoadingSection-Verwandter:
// +0x08 Streckennummer, +0x0C Anzahl, ab +0x10 int16-Nummern) und je Abschnitt die von dort
// sichtbaren Abschnitte (0x34152, Eintrag 148 B: +0x08 Nummer, +0x10 Anzahl, ab +0x14 int16-Liste).
struct TrackRoute {
    int track_number = 0;
    std::vector<int16_t> sections;
    std::map<int, std::vector<int16_t>> visible;
    // Abschnitte, die für die Strecke gezeichnet werden: Strecke + Sichtbarkeitslisten
    // (+ je Abschnitt dessen LOD-Abschnitt, Nummer + 10).
    std::set<int> wanted() const;
};
// Leer bei Erfolg, sonst Fehlertext.
std::string parse_track_route(const uint8_t* buf, size_t len, TrackRoute& out);

}  // namespace nfsu
