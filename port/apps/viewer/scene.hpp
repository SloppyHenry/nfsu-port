// Strecken-Viewer (WP-18): lädt eine Strecke aus den Originaldateien und backt alle
// Szenerie-Instanzen in Weltkoordinaten. Kein Streaming (alle Abschnitte auf einmal),
// keine LOD-Wahl (immer die erste vorhandene Stufe aus SceneryInfo::NameHash[0..3]).
#pragma once
#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

#include "core/paths.hpp"
#include "formats/track_info.hpp"

namespace viewer {

// Vertex im Format von fx/world.efx (24 B): Position, Farbe (RGBA8, aus D3DCOLOR umgestellt), UV.
struct Vertex {
    float x, y, z;
    uint8_t r, g, b, a;
    float u, v;
};
static_assert(sizeof(Vertex) == 24, "world.efx-Vertex");

// Aus TextureInfo::AlphaUsageType/AlphaBlendType (Enums aus nfsmw Texture.hpp, Verhalten wie eSetBlendMode):
// Blend 0 = deckend (Usage 1 = Alphatest), 1 = SrcAlpha/InvSrcAlpha, 2 = additiv; 3..8 wie 1 genähert.
enum class AlphaMode : uint8_t { Opaque, Test, Blend, Additive };

struct Texture {
    int width = 0, height = 0;
    std::vector<uint8_t> rgba;
    AlphaMode alpha = AlphaMode::Opaque;
    std::string name;  // DebugName aus TextureInfo
    uint8_t usage = 0, blend = 0;  // AlphaUsageType, AlphaBlendType
};

// Zeichenbereich eines Abschnitts mit einer Textur.
struct Batch {
    uint32_t first_index = 0, index_count = 0;
    int texture = -1;  // Index in Scene::textures, -1 = keine (weiß)
};

struct Section {
    int number = 0;
    std::string name;
    float bmin[3] = {1e30f, 1e30f, 1e30f}, bmax[3] = {-1e30f, -1e30f, -1e30f};
    std::vector<Batch> batches;
    bool visible = true;
};

struct Stats {
    size_t solids = 0, solids_skipped = 0, instances = 0, instances_missing = 0;
    size_t sections_filtered = 0;  // Anzahl gewünschter Abschnitte laut TRACKB (0 = kein Filter)
    size_t texture_refs = 0, textures_missing = 0;
};

struct Scene {
    nfsu::TrackInfo track;
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    std::vector<Texture> textures;
    std::vector<Section> sections;
    Stats stats;
    std::vector<int> route_sections;  // Abschnitte der Strecke laut TRACKB in Dateireihenfolge
    std::vector<std::string> files;  // tatsächlich geladene Dateien
};

// Alle Strecken aus GLOBAL\GLOBALB.BUN. Leer bei Erfolg, sonst Fehlertext.
std::string list_tracks(nfsu::DataPaths& paths, std::vector<nfsu::TrackInfo>& out);

// Lädt Strecke number. Leer bei Erfolg, sonst Fehlertext.
// all_sections: ohne Streckenführung alle Abschnitte der Region laden.
std::string load_scene(nfsu::DataPaths& paths, int number, Scene& out, bool all_sections = false);

}  // namespace viewer
