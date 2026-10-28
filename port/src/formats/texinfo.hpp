// Dateigebundene Strukturen der Texturpakete (Chunks 0x3331xxxx). Layout aus gc_types.h,
// am PC gegen alle 347 Pakete geprüft (WP-11, WP-13). Zeigerfelder sind ptr32 (ADR-001).
#pragma once
#include <cstddef>
#include <cstdint>

#include "core/ptr32.hpp"

namespace nfsu {

struct TexturePack;  // Laufzeitobjekt, native Zeiger, nicht dateigebunden

#pragma pack(push, 1)
// PC-eigene Plattforminfo, Chunk 0x33310005, je 0x20 Byte, parallel zur TextureInfo-Tabelle.
// Felder +0x08..+0x10 sind in allen Paketen (1|0, 5, 6|2); ihre Bedeutung ist offen.
struct TextureInfoPlatInfoDisk {
    uint32_t node_next;       // +0x00 Listenknoten der erzeugten Texturen (0x77f5c0), in der Datei 0
    uint32_t node_prev;       // +0x04
    uint32_t unk08;           // +0x08 0 oder 1
    uint32_t unk0c;           // +0x0C immer 5
    uint32_t unk10;           // +0x10 6 oder 2
    uint32_t d3d_format;      // +0x14 D3DFORMAT für CreateTexture (DXT1, DXT3, A8R8G8B8, P8)
    uint32_t gpu_texture;     // +0x18 IDirect3DTexture9*; im Port ein Handle (ADR-001), in der Datei 0
    uint32_t unk1c;           // +0x1C immer 0
};

// orig: TextureInfo (gc_types.h), Chunk 0x33310004
struct TextureInfoDisk {
    ptr32<TextureInfoPlatInfoDisk> plat_info;  // +0x00 TextureInfoPlatInterface::PlatInfo
    ptr32<TextureInfoDisk> node_next;  // +0x04 bNode::Next, in der Datei 0
    ptr32<TextureInfoDisk> node_prev;  // +0x08 bNode::Prev, in der Datei 0
    char debug_name[24];      // +0x0C
    uint32_t name_hash;       // +0x24
    uint32_t class_name_hash; // +0x28
    uint32_t image_parent_hash;  // +0x2C
    int32_t image_placement;  // +0x30 Offset im (entpackten) Datenbereich des Pakets
    int32_t palette_placement;// +0x34
    int32_t image_size;       // +0x38 alle Mip-Stufen
    int32_t palette_size;     // +0x3C
    int32_t base_image_size;  // +0x40
    int16_t width, height;    // +0x44
    int8_t shift_width, shift_height;  // +0x48 log2 der Kantenlängen
    uint8_t image_compression_type;    // +0x4A 0x22 DXT1, 0x24 DXT3, 0x20 ARGB, 0x08/0x80/0x81 P8
    uint8_t palette_compression_type;  // +0x4B
    int16_t num_palette_entries;       // +0x4C
    int8_t num_mipmap_levels;          // +0x4E
    int8_t tilable_uv;                 // +0x4F
    int8_t bias_level;                 // +0x50
    int8_t rendering_order;            // +0x51
    int8_t scroll_type;                // +0x52
    int8_t used_flag;                  // +0x53
    int8_t apply_alpha_sorting;        // +0x54
    int8_t alpha_usage_type;           // +0x55
    int8_t alpha_blend_type;           // +0x56
    int8_t write_z;                    // +0x57 bWriteZ
    int16_t scroll_time_step;          // +0x58
    int16_t scroll_speed_s, scroll_speed_t;  // +0x5A
    int16_t offset_s, offset_t;        // +0x5E
    int16_t scale_s, scale_t;          // +0x62
    int16_t pad66;                     // +0x66 Ausrichtung
    ptr32<TexturePack> texture_pack;   // +0x68 beim Laden gesetzt
    ptr32<TextureInfoDisk> image_parent;  // +0x6C
    ptr32<void> image_data;   // +0x70
    ptr32<void> palette_data; // +0x74
    int32_t reference_count;  // +0x78
};

// orig: TexturePackHeader, Chunk 0x33310001. PC weicht vom GC ab: Name 28 statt 32 Byte,
// dafür der Dateinamenshash bei +0x60; ab +0x64 wie gc_types.h (Lader 0x529410 schreibt +0x6C..+0x78).
struct TexturePackHeaderDisk {
    int32_t version;          // +0x00 4 in allen PC-Paketen
    char name[28];            // +0x04
    char filename[64];        // +0x20 z. B. "Global\Art\Cars\240SX\240SX_Textures.tpk"
    uint32_t filename_hash;   // +0x60 name_hash(filename)
    uint32_t perm_chunk_byte_offset;  // +0x64
    uint32_t perm_chunk_byte_size;    // +0x68
    int32_t endian_swapped;   // +0x6C vom Lader auf 1 gesetzt
    ptr32<TexturePack> texture_pack;  // +0x70
    ptr32<struct TextureIndexEntryDisk> index_table;      // +0x74 Chunk 0x33310002
    ptr32<struct StreamingEntryDisk> stream_entry_table;  // +0x78 Chunk 0x33310003
};

// orig: TextureIndexEntry (gc_types.h), Chunk 0x33310002
struct TextureIndexEntryDisk {
    uint32_t name_hash;
    ptr32<TextureInfoDisk> texture_info;
};

// orig: eStreamingEntry (Layout aus eStreamPackLoader::InternalLoadStreamingEntry 0x522890),
// Chunk 0x33310003. Je Textur ein JDLZ-gepackter Block mit Bild und Palette.
struct StreamingEntryDisk {
    uint32_t name_hash;       // +0x00
    uint32_t file_offset;     // +0x04 absolut in der Datei
    uint32_t size;            // +0x08 gepackte Größe
    uint8_t unk0c;            // +0x0C
    uint8_t flags;            // +0x0D Bit 0 in allen Paketen gesetzt; 0x10 = geladen (Laufzeit)
    uint16_t ref_count;       // +0x0E Laufzeit
    ptr32<void> data;         // +0x10 Laufzeit
};
#pragma pack(pop)

static_assert(sizeof(TextureInfoPlatInfoDisk) == 0x20);
static_assert(offsetof(TextureInfoPlatInfoDisk, d3d_format) == 0x14);
static_assert(sizeof(TextureInfoDisk) == 0x7c);
static_assert(offsetof(TextureInfoDisk, name_hash) == 0x24);
static_assert(offsetof(TextureInfoDisk, width) == 0x44);
static_assert(offsetof(TextureInfoDisk, image_compression_type) == 0x4a);
static_assert(offsetof(TextureInfoDisk, num_mipmap_levels) == 0x4e);
static_assert(offsetof(TextureInfoDisk, scroll_time_step) == 0x58);
static_assert(offsetof(TextureInfoDisk, scale_t) == 0x64);
static_assert(offsetof(TextureInfoDisk, texture_pack) == 0x68);
static_assert(offsetof(TextureInfoDisk, reference_count) == 0x78);
static_assert(sizeof(TexturePackHeaderDisk) == 0x7c);
static_assert(offsetof(TexturePackHeaderDisk, filename_hash) == 0x60);
static_assert(offsetof(TexturePackHeaderDisk, endian_swapped) == 0x6c);
static_assert(offsetof(TexturePackHeaderDisk, stream_entry_table) == 0x78);
static_assert(sizeof(TextureIndexEntryDisk) == 8);
static_assert(sizeof(StreamingEntryDisk) == 0x14);

}  // namespace nfsu
