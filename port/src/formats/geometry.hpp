// Solid-Geometrie (Autos, Streckenobjekte) aus Chunks 0x134xxx ("eSolid"/"eModel").
// Namen aus der nicht gestrippten GC-Speed.elf (eSolid, eSolidListHeader, eModel), Layout am PC
// gegen GEOMETRY.BIN und Streckenpakete geprüft (WP-14). orig:
//   0x523f30 InternalLoaderSolidHeaderChunks  (0x134002 eSolidListHeader, 0x134003/4 Tabellen)
//   0x524010 FUN_00524010                     (0x134011 eSolid, 0x134012/13 Texturen/Licht)
//   0x40b760 FUN_0040b760                     (0x134900 eModel, 0x134b01..03 Vertex-/Indexdaten)
#pragma once
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

namespace nfsu {

// Zwei Vertexformate gefunden (Byte-genau geprüft: Einheitsnormalen, Indexgrenzen, Lagewerte):
//   36 B: Position(3f) Normale(3f) Farbe(D3DCOLOR) UV(2f)   -- Autoteile
//   24 B: Position(3f) Farbe(D3DCOLOR) UV(2f)                -- einfache Streckenobjekte, keine Normale
//   60 B: wie 36 B + Blendgewichte(3f) + Blendindizes(3f)    -- animierte Streckenobjekte (worldbone.efx)
struct GeomVertex {
    float px = 0, py = 0, pz = 0;
    float nx = 0, ny = 0, nz = 1;
    uint32_t color = 0xffffffffu;  // D3DCOLOR (A<<24|R<<16|G<<8|B)
    float u = 0, v = 0;
};

// Draw-Batch aus der StripEntry-Tabelle (0x134b02, 0x3c B je Eintrag): Indexanzahl +0xc,
// Texturtabellenindex +0x1c, Startindex +0x34 (Einheit Indizes). Gesichert durch den
// DrawIndexedPrimitive-Aufruf in 0x40dd10 (primCount = Anzahl/3, Dreiecksliste).
struct GeomBatch {
    uint32_t start = 0, count = 0;
    uint32_t texture_hash = 0;
};

struct GeomSolid {
    uint32_t name_hash = 0;  // Feld +0x10 des Solid-Kopfs (nur teilweise = name_hash(name))
    std::string name;        // Solidname am Ende von 0x134011
    uint32_t key_hash = 0;   // name_hash(name): Schlüssel der Szenerie (SceneryInfo::NameHash)
    uint32_t texture_hash = 0;  // erster Eintrag aus 0x134012, 0 wenn keine Textur
    std::vector<uint32_t> texture_hashes;  // alle Einträge aus 0x134012 (8 B je Eintrag)
    std::vector<GeomBatch> batches;        // leer = ganzes Solid mit texture_hash
    bool has_normal = false;
    std::vector<GeomVertex> vertices;
    std::vector<uint16_t> indices;  // Dreiecksliste (je 3 ein Dreieck)
};

// Liest alle eSolid/eModel-Objekte aus einem (entpackten) Chunkpuffer (GEOMETRY.BIN oder
// Streckenpaket). Solids ohne erkennbares Vertexformat werden ausgelassen (siehe skipped).
// Leer bei Erfolg (auch wenn skipped > 0), sonst Fehlertext bei einem Strukturfehler.
std::string parse_geometry(const uint8_t* buf, size_t len, std::vector<GeomSolid>& out, size_t* skipped = nullptr);

}  // namespace nfsu
