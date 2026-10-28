#include "formats/geometry.hpp"

#include <cmath>
#include <cstring>
#include <limits>

#include "core/hash.hpp"
#include "formats/chunk.hpp"

namespace nfsu {

static uint32_t rd32(const uint8_t* p) { uint32_t v; std::memcpy(&v, p, 4); return v; }
static float rdf(const uint8_t* p) { float v; std::memcpy(&v, p, 4); return v; }

namespace {

// Nimmt an, der Puffer liegt im Original an einer 128-Byte-Grenze; Füllbytes (0x11) davor
// überspringen. Geprüft an 395 Solids aus Cars/350Z/GEOMETRY.BIN (Index- und Normalenprobe).
size_t pad_to(size_t file_offset, size_t align) {
    size_t rem = file_offset % align;
    return rem ? align - rem : 0;
}

bool sane_vertex(const uint8_t* buf, size_t stride, size_t n) {
    for (size_t i = 0; i < n; ++i) {
        const uint8_t* p = buf + i * stride;
        for (int c = 0; c < 3; ++c) {
            float f = rdf(p + c * 4);
            if (!std::isfinite(f) || std::fabs(f) > 1e5f) return false;
        }
    }
    return true;
}

// Liest ein Solid mit erkanntem Vertexformat (36 B mit Normale, sonst 24 B ohne) aus den
// bereits ausgerichteten Vertex-/Indexpuffern. leer = Format nicht erkannt.
bool decode_model(const uint8_t* vbuf, size_t vsize, const uint8_t* ibuf, size_t isize, GeomSolid& solid) {
    if (isize < 2) return false;
    size_t nidx = isize / 2;
    solid.indices.resize(nidx);
    uint16_t maxidx = 0;
    for (size_t i = 0; i < nidx; ++i) {
        uint16_t v;
        std::memcpy(&v, ibuf + i * 2, 2);
        solid.indices[i] = v;
        if (v > maxidx) maxidx = v;
    }
    for (size_t stride : {size_t(36), size_t(24), size_t(60)}) {
        if (vsize == 0 || vsize % stride != 0) continue;
        size_t n = vsize / stride;
        if (n == 0 || maxidx >= n) continue;
        if (!sane_vertex(vbuf, stride, n)) continue;
        solid.has_normal = (stride != 24);  // 60 B = 36 B + BLENDW(3f) + BLENDI(3f), Gewichte ignoriert
        solid.vertices.resize(n);
        for (size_t i = 0; i < n; ++i) {
            const uint8_t* p = vbuf + i * stride;
            GeomVertex& v = solid.vertices[i];
            v.px = rdf(p); v.py = rdf(p + 4); v.pz = rdf(p + 8);
            if (solid.has_normal) {
                v.nx = rdf(p + 12); v.ny = rdf(p + 16); v.nz = rdf(p + 20);
                v.color = rd32(p + 24);
                v.u = rdf(p + 28); v.v = rdf(p + 32);
            } else {
                v.nx = 0; v.ny = 0; v.nz = 1;
                v.color = rd32(p + 12);
                v.u = rdf(p + 16); v.v = rdf(p + 20);
            }
        }
        return true;
    }
    return false;
}

}  // namespace

std::string parse_geometry(const uint8_t* buf, size_t len, std::vector<GeomSolid>& out, size_t* skipped) {
    size_t skip_count = 0;
    GeomSolid cur;
    bool have_solid = false;
    const uint8_t* vbuf = nullptr; size_t vbuf_size = 0, vbuf_foff = 0;
    bool vbuf_set = false;
    const uint8_t* strips = nullptr; size_t nstrips = 0;

    auto flush = [&]() {
        if (have_solid && !cur.vertices.empty() && !cur.indices.empty()) out.push_back(cur);
        else if (have_solid) ++skip_count;  // leere Platzhalter (nur Füllbytes als Vertexpuffer, keine Indizes)
        cur = GeomSolid{};
        have_solid = false;
        vbuf_set = false;
    };

    std::string err = walk_chunks(buf, len, [&](const ChunkView& c) {
        switch (c.id) {
        case 0x80134010:
            flush();
            have_solid = true;
            break;
        case 0x134011:
            if (have_solid && c.size >= 0x14) cur.name_hash = rd32(c.data + 0x10);
            if (have_solid) {
                size_t e = c.size;
                while (e > 0 && c.data[e - 1] == 0) --e;
                size_t b = e;
                while (b > 0 && c.data[b - 1] >= 0x20 && c.data[b - 1] < 0x7f) --b;
                cur.name.assign(reinterpret_cast<const char*>(c.data + b), e - b);
                cur.key_hash = name_hash(cur.name.c_str());
            }
            break;
        case 0x134012:
            if (have_solid && cur.texture_hash == 0 && c.size >= 4) cur.texture_hash = rd32(c.data);
            if (have_solid && cur.texture_hashes.empty())
                for (size_t o = 0; o + 8 <= c.size; o += 8) cur.texture_hashes.push_back(rd32(c.data + o));
            break;
        case 0x134b02:
            strips = c.data + pad_to(size_t(c.data - buf), 16);
            nstrips = (c.size - pad_to(size_t(c.data - buf), 16)) / 0x3c;
            break;
        case 0x80134100:
            strips = nullptr; nstrips = 0;
            vbuf_set = false;  // neues eModel: vorherige Vertexdaten verwerfen (nur 1. Modell je Solid)
            break;
        case 0x134b01:
            vbuf = c.data; vbuf_size = c.size; vbuf_foff = size_t(c.data - buf); vbuf_set = true;
            break;
        case 0x134b03:
            if (have_solid && vbuf_set && cur.vertices.empty()) {
                size_t vpad = pad_to(vbuf_foff, 128);
                size_t ipad = pad_to(size_t(c.data - buf), 16);
                const uint8_t* vp = vbuf + vpad;
                size_t vs = vpad < vbuf_size ? vbuf_size - vpad : 0;
                const uint8_t* ip = c.data + ipad;
                size_t is = ipad < c.size ? c.size - ipad : 0;
                if (decode_model(vp, vs, ip, is, cur) && strips) {
                    for (size_t k = 0; k < nstrips; ++k) {
                        const uint8_t* e = strips + k * 0x3c;
                        GeomBatch b;
                        b.count = rd32(e + 0xc);
                        b.start = rd32(e + 0x34);
                        uint32_t ti = rd32(e + 0x1c);
                        b.texture_hash = ti < cur.texture_hashes.size() ? cur.texture_hashes[ti] : cur.texture_hash;
                        if (b.count == 0 || size_t(b.start) + b.count > cur.indices.size()) { cur.batches.clear(); break; }
                        cur.batches.push_back(b);
                    }
                }  // scheitert: cur.vertices bleibt leer, flush() zaehlt
            }
            break;
        default: break;
        }
        return true;
    });
    flush();
    if (skipped) *skipped = skip_count;
    return err;
}

}  // namespace nfsu
