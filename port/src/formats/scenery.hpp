// Szenerie-Abschnitte der Streckenpakete (Container 0x80034100 "ScenerySection").
// Namen und Layout aus der GC-Speed.elf (ScenerySectionHeader, SceneryInfo, SceneryInstance,
// SceneryTreeNode in symbols/gc_types.h), gelesen nach GC LoaderScenery 0x800447A0. Der PC-Lader
// liegt in einer von Ghidra nicht erkannten Lücke (0x442ab0..0x442ec0); das Layout wurde deshalb
// an STREAML1R{A,B,C,E,X}.BUN geprüft (WP-18): Indizes im Bereich, Rotationszeilen ≈ 8192·Skala,
// 99,6 % der Instanzen in RA finden ihr Solid über SceneryInfo::NameHash.
#pragma once
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

#include "core/ptr32.hpp"

namespace nfsu {

#pragma pack(push, 1)
// Chunk 0x34101, 16-Byte-ausgerichtet im Chunk. Nur SectionNumber ist in der Datei belegt,
// die übrigen Felder setzt der Lader.
struct ScenerySectionHeaderDisk {
    ptr32<void> node_next, node_prev;  // +0x00 bTNode
    int32_t endian_swapped;            // +0x08
    int32_t section_number;            // +0x0C z. B. 101 = A1, 2601 = Bibliothek (/100 == 26)
    int32_t num_polygons_in_memory;    // +0x10
    int32_t num_polygons_in_world;     // +0x14
    ptr32<void> scenery_info;          // +0x18
    int32_t num_scenery_info;          // +0x1C
    ptr32<void> scenery_instance;      // +0x20
    int32_t num_scenery_instances;     // +0x24
    ptr32<void> tree_nodes;            // +0x28
    int32_t num_tree_nodes;            // +0x2C
    ptr32<void> tree_group_nodes;      // +0x30
    int32_t num_tree_group_nodes;      // +0x34
    int32_t views_visible_this_frame;  // +0x38
};
static_assert(sizeof(ScenerySectionHeaderDisk) == 0x3c, "ScenerySectionHeader");

// Chunk 0x34102, nicht ausgerichtet. NameHash[0..3] = LOD-Stufen, [4]/[5] für ExcludeFlags 0x800/0x1000.
struct SceneryInfoDisk {
    uint32_t name_hash[6];        // +0x00 eSolid-Namenshash, 0 = leer
    int16_t far_clip_size[4];     // +0x18 Pixelgröße, ab der die Stufe gilt
    int16_t far_clip_size_4;      // +0x20 "Dummy" (Schwelle für pModel[4])
    int16_t far_clip_size_5;      // +0x22 Füllfeld im GC-Typ, Schwelle für pModel[5] (DrawAScenery)
    ptr32<void> model[6];         // +0x24 eModel*, setzt der Lader
    int8_t is_facade[6];          // +0x3C
    int8_t dummy2;                // +0x42
    int8_t pad43;                 // +0x43
    float radius;                 // +0x44
};
#pragma pack(pop)
static_assert(sizeof(SceneryInfoDisk) == 0x48, "SceneryInfo");

#pragma pack(push, 1)
// Chunk 0x34103, 16-Byte-ausgerichtet im Chunk.
struct SceneryInstanceDisk {
    int16_t bbox_min[3], bbox_max[3];  // +0x00 SceneryBoundingBox (ganze Meter)
    int16_t scenery_info_number;       // +0x0C Index in SceneryInfo
    uint16_t exclude_flags;            // +0x0E 0x200 = Solid liegt schon in Weltkoordinaten
    float position[3];                 // +0x10
    int16_t rotation[9];               // +0x1C 3×3-Zeilen, Einheit 1/8192
    int16_t pad;                       // +0x2E
};
// Chunk 0x34104
struct SceneryTreeNodeDisk {
    int16_t bbox_min[3], bbox_max[3];
    int16_t num_children;
    int16_t child_codes[5];
};
#pragma pack(pop)
static_assert(sizeof(SceneryInstanceDisk) == 0x30, "SceneryInstance");
static_assert(sizeof(SceneryTreeNodeDisk) == 0x18, "SceneryTreeNode");

constexpr uint16_t kSceneryExcludeIdentity = 0x200;  // orig DrawAScenery 0x443140: keine Matrix

struct ScenerySection {
    int number = 0;
    std::vector<SceneryInfoDisk> infos;
    std::vector<SceneryInstanceDisk> instances;
};

// orig: LoaderScenery (GC 0x800447A0), ohne Modellsuche und Rückwärts-Spiegelung.
// Liest alle Abschnitte eines (entpackten) Streckenpakets. Leer bei Erfolg, sonst Fehlertext.
std::string parse_scenery(const uint8_t* buf, size_t len, std::vector<ScenerySection>& out);

// orig: 0x442880 (Instanzmatrix aus Rotation/8192 und Position), D3D-Zeilenkonvention:
// m[0..2] = Zeile 0, m[12..14] = Position. Bei kSceneryExcludeIdentity Einheitsmatrix.
void scenery_instance_matrix(const SceneryInstanceDisk& inst, float m[16]);

// Abschnittsname wie im Spiel: 101 -> "A1", 2601 -> "Z1" (Buchstabe = Nummer/100, ab 1 = A).
std::string scenery_section_name(int number);

}  // namespace nfsu
