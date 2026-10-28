#include "formats/texture_pack.hpp"

#include <algorithm>
#include <cstring>

#include "formats/chunk.hpp"
#include "formats/jdlz.hpp"

namespace nfsu {

static uint32_t rd32(const uint8_t* p) { uint32_t v; std::memcpy(&v, p, 4); return v; }
static uint16_t rd16(const uint8_t* p) { uint16_t v; std::memcpy(&v, p, 2); return v; }

std::string TexturePackView::name() const {
    return header ? std::string(header->name, strnlen(header->name, sizeof header->name)) : std::string();
}

std::string find_texture_packs(const uint8_t* buf, size_t len, std::vector<TexturePackView>& out) {
    TexturePackView cur;
    bool in_header = false;
    std::string err;
    std::string walk_err = walk_chunks(buf, len, [&](const ChunkView& c) {
        switch (c.id) {
        case 0xb3310000: cur = {}; in_header = true; break;
        case 0x33310001:
            if (c.size >= sizeof(TexturePackHeaderDisk)) cur.header = reinterpret_cast<const TexturePackHeaderDisk*>(c.data);
            break;
        case 0x33310002:
            cur.index = reinterpret_cast<const TextureIndexEntryDisk*>(c.data);
            cur.num_index = c.size / sizeof(TextureIndexEntryDisk);
            break;
        case 0x33310003:
            cur.streams = reinterpret_cast<const StreamingEntryDisk*>(c.data);
            cur.num_streams = c.size / sizeof(StreamingEntryDisk);
            break;
        case 0x33310004:
            cur.infos = reinterpret_cast<const TextureInfoDisk*>(c.data);
            cur.num_textures = c.size / sizeof(TextureInfoDisk);
            break;
        case 0x33310005:
            cur.plats = reinterpret_cast<const TextureInfoPlatInfoDisk*>(c.data);
            if (c.size / sizeof(TextureInfoPlatInfoDisk) != cur.num_textures && err.empty())
                err = "PlatInfo- und TextureInfo-Zahl verschieden bei " + std::to_string(c.offset);
            break;
        case 0x33320002: {
            if (!in_header) break;
            in_header = false;
            if (!cur.header || (cur.num_textures && !cur.plats)) {
                if (err.empty()) err = "unvollständiger Paketkopf vor " + std::to_string(c.offset);
                break;
            }
            // Datenbasis: erste 0x80-Grenze ab Nutzdatenbeginn (Füllbytes 0x11, vgl. 0x4451d0)
            size_t pos = size_t(c.data - buf);
            size_t pad = ((pos + 0x7f) & ~size_t(0x7f)) - pos;
            cur.data = c.data + std::min<size_t>(pad, c.size);
            cur.data_size = c.size > pad ? c.size - pad : 0;
            cur.file = buf;
            cur.file_size = len;
            out.push_back(cur);
            break;
        }
        default: break;
        }
        return true;
    });
    if (!walk_err.empty()) return walk_err;
    return err;
}

std::string texture_bytes(const TexturePackView& pack, size_t i, std::vector<uint8_t>& scratch, TextureBytes& out) {
    out = {};
    const TextureInfoDisk& t = pack.infos[i];
    const bool has_pal = t.palette_size > 0;
    if (t.image_size <= 0) return "Bildgröße 0";

    const uint8_t* base = pack.data;   // Basis der Placements
    size_t base_size = pack.data_size;
    int32_t origin = 0;                // Placement, das bei base liegt
    if (pack.streams) {
        // Einzelkompression: der Block beginnt beim kleinsten Placement der Textur
        const StreamingEntryDisk* e = nullptr;
        for (size_t k = 0; k < pack.num_streams; ++k)
            if (pack.streams[k].name_hash == t.name_hash) { e = &pack.streams[k]; break; }
        if (!e) return "kein Streaming-Eintrag";
        if (size_t(e->file_offset) + e->size > pack.file_size || e->size < 0x12) return "Streaming-Eintrag außerhalb der Datei";
        const uint8_t* src = pack.file + e->file_offset;
        if (rd32(src) != 0x5a4c444au || src[4] != 2 || rd32(src + 12) != e->size) return "Streaming-Block ist kein JDLZ";
        const uint32_t un = rd32(src + 8);
        scratch.assign(size_t(un) + 0x1100, 0);  // Kopien werden im Original nicht begrenzt (jdlz.hpp)
        if (jdlz_decompress(src, scratch.data()) != un) return "JDLZ-Fehler";
        base = scratch.data();
        base_size = un;
        origin = has_pal ? std::min(t.image_placement, t.palette_placement) : t.image_placement;
    }
    auto inside = [&](int32_t off, int32_t size) {
        return off >= origin && size_t(off - origin) + size_t(size) <= base_size;
    };
    if (!inside(t.image_placement, t.image_size)) return "Bild außerhalb des Datenbereichs";
    if (has_pal && !inside(t.palette_placement, t.palette_size)) return "Palette außerhalb des Datenbereichs";
    out.image = base + (t.image_placement - origin);
    if (has_pal) out.palette = base + (t.palette_placement - origin);
    return {};
}

size_t mip_level_size(uint32_t fmt, int w, int h) {
    w = std::max(w, 1);
    h = std::max(h, 1);
    switch (fmt) {
    case kD3DFmtDXT1: return size_t((w + 3) / 4) * ((h + 3) / 4) * 8;
    case kD3DFmtDXT3:
    case kD3DFmtDXT5: return size_t((w + 3) / 4) * ((h + 3) / 4) * 16;
    case kD3DFmtA8R8G8B8: return size_t(w) * h * 4;
    case kD3DFmtP8: return size_t(w) * h;
    default: return 0;
    }
}

// RGB565 nach RGBA8 mit Bitwiederholung (wie D3D-Referenz)
static void rgb565(uint16_t c, uint8_t* o) {
    uint8_t r = (c >> 11) & 31, g = (c >> 5) & 63, b = c & 31;
    o[0] = uint8_t(r << 3 | r >> 2);
    o[1] = uint8_t(g << 2 | g >> 4);
    o[2] = uint8_t(b << 3 | b >> 2);
    o[3] = 255;
}

// Farbteil eines DXT-Blocks. four = immer 4-Farben-Modus (DXT3/5).
static void dxt_color(const uint8_t* src, uint8_t* dst, size_t stride, bool four) {
    uint16_t c0 = rd16(src), c1 = rd16(src + 2);
    uint8_t pal[4][4];
    rgb565(c0, pal[0]);
    rgb565(c1, pal[1]);
    if (four || c0 > c1) {
        for (int k = 0; k < 3; ++k) {
            pal[2][k] = uint8_t((2 * pal[0][k] + pal[1][k]) / 3);
            pal[3][k] = uint8_t((pal[0][k] + 2 * pal[1][k]) / 3);
        }
        pal[2][3] = pal[3][3] = 255;
    } else {
        for (int k = 0; k < 3; ++k) pal[2][k] = uint8_t((pal[0][k] + pal[1][k]) / 2);
        pal[2][3] = 255;
        pal[3][0] = pal[3][1] = pal[3][2] = pal[3][3] = 0;
    }
    uint32_t bits = rd32(src + 4);
    for (int y = 0; y < 4; ++y)
        for (int x = 0; x < 4; ++x, bits >>= 2) std::memcpy(dst + y * stride + x * 4, pal[bits & 3], 4);
}

void decode_dxt1_block(const uint8_t* src, uint8_t* dst, size_t stride) { dxt_color(src, dst, stride, false); }

void decode_dxt3_block(const uint8_t* src, uint8_t* dst, size_t stride) {
    dxt_color(src + 8, dst, stride, true);
    for (int y = 0; y < 4; ++y) {
        uint16_t row = rd16(src + 2 * y);
        for (int x = 0; x < 4; ++x) dst[y * stride + x * 4 + 3] = uint8_t(((row >> (4 * x)) & 15) * 17);
    }
}

void decode_dxt5_block(const uint8_t* src, uint8_t* dst, size_t stride) {
    dxt_color(src + 8, dst, stride, true);
    uint8_t a[8] = {src[0], src[1]};
    if (a[0] > a[1]) {
        for (int k = 1; k < 7; ++k) a[k + 1] = uint8_t(((7 - k) * a[0] + k * a[1]) / 7);
    } else {
        for (int k = 1; k < 5; ++k) a[k + 1] = uint8_t(((5 - k) * a[0] + k * a[1]) / 5);
        a[6] = 0;
        a[7] = 255;
    }
    uint64_t bits = 0;
    for (int k = 0; k < 6; ++k) bits |= uint64_t(src[2 + k]) << (8 * k);
    for (int y = 0; y < 4; ++y)
        for (int x = 0; x < 4; ++x, bits >>= 3) dst[y * stride + x * 4 + 3] = a[bits & 7];
}

std::string decode_texture_rgba(const TextureInfoDisk& t, uint32_t fmt, const TextureBytes& bytes, std::vector<uint8_t>& rgba) {
    const int w = t.width, h = t.height;
    if (w <= 0 || h <= 0) return "Größe 0";
    if (!bytes.image) return "keine Bilddaten";
    const size_t need = mip_level_size(fmt, w, h);
    if (!need) return "unbekanntes Format " + std::to_string(fmt);
    if (need > size_t(t.image_size)) return "Bildgröße zu klein";
    rgba.assign(size_t(w) * h * 4, 0);
    const uint8_t* s = bytes.image;
    switch (fmt) {
    case kD3DFmtA8R8G8B8:   // Speicher: B G R A
        for (size_t p = 0; p < size_t(w) * h; ++p, s += 4) {
            uint8_t* o = &rgba[p * 4];
            o[0] = s[2]; o[1] = s[1]; o[2] = s[0]; o[3] = s[3];
        }
        return {};
    case kD3DFmtP8: {
        // orig 0x4071c0: Index → D3DCOLOR aus PaletteData, Ergebnis als A8R8G8B8
        if (!bytes.palette) return "P8 ohne Palette";
        const size_t n = size_t(t.palette_size) / 4;
        for (size_t p = 0; p < size_t(w) * h; ++p) {
            uint8_t idx = s[p];
            if (idx >= n) return "Palettenindex außerhalb";
            const uint8_t* c = bytes.palette + idx * 4;
            uint8_t* o = &rgba[p * 4];
            o[0] = c[2]; o[1] = c[1]; o[2] = c[0]; o[3] = c[3];
        }
        return {};
    }
    default: break;
    }
    // DXT: Blöcke in ein auf Vielfache von 4 gerundetes Bild, dann zuschneiden
    const int bw = (w + 3) / 4, bh = (h + 3) / 4;
    const size_t bsize = fmt == kD3DFmtDXT1 ? 8 : 16;
    std::vector<uint8_t> tmp(size_t(bw) * 4 * bh * 4 * 4);
    const size_t stride = size_t(bw) * 16;
    for (int by = 0; by < bh; ++by)
        for (int bx = 0; bx < bw; ++bx, s += bsize) {
            uint8_t* d = &tmp[(size_t(by) * 4 * stride) + size_t(bx) * 16];
            if (fmt == kD3DFmtDXT1) decode_dxt1_block(s, d, stride);
            else if (fmt == kD3DFmtDXT3) decode_dxt3_block(s, d, stride);
            else decode_dxt5_block(s, d, stride);
        }
    for (int y = 0; y < h; ++y) std::memcpy(&rgba[size_t(y) * w * 4], &tmp[size_t(y) * stride], size_t(w) * 4);
    return {};
}

}  // namespace nfsu
