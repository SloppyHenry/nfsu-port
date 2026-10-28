// Texturpakete (TPK) in Chunkdateien: Container 0xB3300000 mit Kopfteil 0xB3310000 und
// Datenteil 0xB3320000. Lesen, Bilddaten auffinden (roh oder JDLZ-gepackt je Textur) und nach
// RGBA8 dekodieren. Die GPU-Erzeugung (orig 0x4071c0) folgt im Renderer (WP-40).
#pragma once
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

#include "formats/texinfo.hpp"

namespace nfsu {

// D3DFORMAT-Werte aus TextureInfoPlatInfoDisk::d3d_format
enum : uint32_t {
    kD3DFmtA8R8G8B8 = 21,
    kD3DFmtP8 = 41,
    kD3DFmtDXT1 = 0x31545844,
    kD3DFmtDXT3 = 0x33545844,
    kD3DFmtDXT5 = 0x35545844,
};

// Sicht auf ein Paket im Dateipuffer. Zeiger zeigen in den Puffer und gelten, solange er lebt.
struct TexturePackView {
    const TexturePackHeaderDisk* header = nullptr;
    const TextureIndexEntryDisk* index = nullptr;
    const StreamingEntryDisk* streams = nullptr;   // nur bei Paketen mit Einzelkompression
    const TextureInfoDisk* infos = nullptr;
    const TextureInfoPlatInfoDisk* plats = nullptr;
    size_t num_index = 0, num_streams = 0, num_textures = 0;
    const uint8_t* data = nullptr;   // Datenbereich (0x80-ausgerichtet), Basis der Placements
    size_t data_size = 0;
    const uint8_t* file = nullptr;   // ganzer Puffer: StreamingEntryDisk::file_offset zählt ab hier
    size_t file_size = 0;
    std::string name() const;
};

// orig: 0x529410 InternalLoadTexturePackHeaderChunks (nur das Auffinden der Tabellen; das
// Original verlangt gleich viele TextureInfo- und PlatInfo-Einträge, sonst kein Paket).
// Findet alle Pakete in einem (entpackten) Chunkpuffer. Leer bei Erfolg, sonst Fehlertext.
std::string find_texture_packs(const uint8_t* buf, size_t len, std::vector<TexturePackView>& out);

// orig: 0x529a00 TexturePack::AssignTextureData (Placement → Datenzeiger)
// Bild- und Palettendaten einer Textur. Bei gepackten Paketen wird der Block des
// Streaming-Eintrags nach scratch entpackt. Leer bei Erfolg, sonst Fehlertext.
struct TextureBytes {
    const uint8_t* image = nullptr;
    const uint8_t* palette = nullptr;  // nur P8
};
std::string texture_bytes(const TexturePackView& pack, size_t i, std::vector<uint8_t>& scratch, TextureBytes& out);

// Größe einer Mip-Stufe in Byte für das Format (0 bei unbekanntem Format).
size_t mip_level_size(uint32_t d3d_format, int width, int height);

// orig: 0x4071c0 TextureInfo::CreatePlatTexture_pc (nur die Formatumsetzung)
// Dekodiert Mip-Stufe 0 nach RGBA8 (Zeilen von oben). P8 wie im Original über die
// D3DCOLOR-Palette. Leer bei Erfolg, sonst Fehlertext.
std::string decode_texture_rgba(const TextureInfoDisk& info, uint32_t d3d_format, const TextureBytes& bytes,
                                std::vector<uint8_t>& rgba);

// Einzelne DXT-Blöcke (4×4 Pixel) nach RGBA8; dst hat Zeilenabstand stride Byte.
void decode_dxt1_block(const uint8_t* src, uint8_t* dst, size_t stride);
void decode_dxt3_block(const uint8_t* src, uint8_t* dst, size_t stride);
void decode_dxt5_block(const uint8_t* src, uint8_t* dst, size_t stride);

}  // namespace nfsu
