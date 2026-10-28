#include <cstdint>
#include <cstring>
#include <vector>

#include "formats/geometry.hpp"
#include "test.hpp"

using namespace nfsu;

namespace {

void put32(std::vector<uint8_t>& v, uint32_t x) { uint8_t b[4]; std::memcpy(b, &x, 4); v.insert(v.end(), b, b + 4); }
void putf(std::vector<uint8_t>& v, float x) { uint32_t u; std::memcpy(&u, &x, 4); put32(v, u); }
void put16(std::vector<uint8_t>& v, uint16_t x) { uint8_t b[2]; std::memcpy(b, &x, 2); v.insert(v.end(), b, b + 2); }

size_t pad_to(size_t off, size_t align) { size_t r = off % align; return r ? align - r : 0; }

// Schreibt Kopf {id, size} und data; data selbst beginnt also bei v.size() (vor diesem Aufruf) + 8.
void chunk(std::vector<uint8_t>& v, uint32_t id, const std::vector<uint8_t>& data) {
    put32(v, id); put32(v, uint32_t(data.size())); v.insert(v.end(), data.begin(), data.end());
}

// Vertexdaten mit führenden 0x11-Füllbytes, so dass die Nutzdaten exakt auf eine align-Grenze
// fallen -- wie im Original (geometry.cpp spiegelt dieselbe Ausrichtung beim Lesen).
// header_off: Position, an der {id,size} dieses Chunks geschrieben wird (v.size() beim Aufruf).
std::vector<uint8_t> aligned_payload(size_t header_off, size_t align, const std::vector<uint8_t>& real) {
    size_t data_off = header_off + 8;
    size_t pad = pad_to(data_off, align);
    std::vector<uint8_t> out(pad, 0x11);
    out.insert(out.end(), real.begin(), real.end());
    return out;
}

std::vector<uint8_t> vertex_floats_with_normal() {
    std::vector<uint8_t> v;
    struct V { float p[3], n[3]; uint32_t c; float uv[2]; };
    V verts[3] = {
        {{0, 0, 0}, {0, 0, 1}, 0xffff0000u, {0, 0}},
        {{1, 0, 0}, {0, 0, 1}, 0xff00ff00u, {1, 0}},
        {{0, 1, 0}, {0, 0, 1}, 0xff0000ffu, {0, 1}},
    };
    for (auto& vt : verts) {
        putf(v, vt.p[0]); putf(v, vt.p[1]); putf(v, vt.p[2]);
        putf(v, vt.n[0]); putf(v, vt.n[1]); putf(v, vt.n[2]);
        put32(v, vt.c);
        putf(v, vt.uv[0]); putf(v, vt.uv[1]);
    }
    return v;
}

std::vector<uint8_t> vertex_floats_no_normal() {
    std::vector<uint8_t> v;
    struct V { float p[3]; uint32_t c; float uv[2]; };
    V verts[3] = {
        {{0, 0, 0}, 0xffffffffu, {0, 0}},
        {{2, 0, 0}, 0xffffffffu, {1, 0}},
        {{0, 2, 0}, 0xffffffffu, {0, 1}},
    };
    for (auto& vt : verts) {
        putf(v, vt.p[0]); putf(v, vt.p[1]); putf(v, vt.p[2]);
        put32(v, vt.c);
        putf(v, vt.uv[0]); putf(v, vt.uv[1]);
    }
    return v;
}

std::vector<uint8_t> index_triangle() {
    std::vector<uint8_t> v;
    put16(v, 0); put16(v, 1); put16(v, 2);
    return v;
}

// Baut einen vollständigen 0x80134010-Solid-Block (eSolid + ein eModel) an die aktuelle Position
// von root an (für die Ausrichtung von Vertex-/Indexdaten wichtig).
void append_solid(std::vector<uint8_t>& root, uint32_t name_hash, uint32_t tex_hash,
                  const std::vector<uint8_t>& vertex_floats, const std::vector<uint8_t>& index_bytes) {
    std::vector<uint8_t> h011(176, 0);
    std::memcpy(h011.data() + 0x10, &name_hash, 4);
    std::vector<uint8_t> h012(8, 0);
    std::memcpy(h012.data(), &tex_hash, 4);
    std::vector<uint8_t> h900(64, 0x11);  // Füllmuster wie im Original (Laufzeitfelder)

    std::vector<uint8_t> model;
    chunk(model, 0x134900u, h900);

    // Ausrichtung relativ zur ENDGÜLTIGEN Position in root: solid+model-Kopfe vorab durchrechnen.
    size_t pos = root.size() + 8 /*80134010 Kopf*/ + 8 /*134011 Kopf*/ + h011.size()
                + 8 /*134012 Kopf*/ + h012.size() + 8 /*80134100 Kopf*/ + 8 /*134900 Kopf*/ + h900.size()
                + 8 /*134b01 Kopf*/;
    auto vbuf = aligned_payload(pos - 8, 128, vertex_floats);
    chunk(model, 0x134b01u, vbuf);

    pos = root.size() + 8 + 8 + h011.size() + 8 + h012.size() + 8 + model.size() + 8;
    auto ibuf = aligned_payload(pos - 8, 16, index_bytes);
    chunk(model, 0x134b03u, ibuf);

    std::vector<uint8_t> solid;
    chunk(solid, 0x134011u, h011);
    chunk(solid, 0x134012u, h012);
    chunk(solid, 0x80134100u, model);

    chunk(root, 0x80134010u, solid);
}

}  // namespace

TEST(geometry_normal_format) {
    std::vector<uint8_t> root;
    append_solid(root, 0xaabbccddu, 0x12345678u, vertex_floats_with_normal(), index_triangle());

    std::vector<GeomSolid> out;
    size_t skipped = 0;
    std::string err = parse_geometry(root.data(), root.size(), out, &skipped);
    CHECK(err.empty());
    CHECK(skipped == 0);
    CHECK(out.size() == 1);
    CHECK(out[0].name_hash == 0xaabbccddu);
    CHECK(out[0].texture_hash == 0x12345678u);
    CHECK(out[0].has_normal);
    CHECK(out[0].vertices.size() == 3);
    CHECK(out[0].indices.size() == 3);
    CHECK(out[0].vertices[1].px == 1.0f);
    CHECK(out[0].vertices[2].color == 0xff0000ffu);
    CHECK(out[0].indices[0] == 0 && out[0].indices[2] == 2);
}

TEST(geometry_no_normal_format) {
    std::vector<uint8_t> root;
    append_solid(root, 1, 0, vertex_floats_no_normal(), index_triangle());

    std::vector<GeomSolid> out;
    std::string err = parse_geometry(root.data(), root.size(), out);
    CHECK(err.empty());
    CHECK(out.size() == 1);
    CHECK(!out[0].has_normal);
    CHECK(out[0].vertices.size() == 3);
    CHECK(out[0].vertices[1].px == 2.0f);
    CHECK(out[0].texture_hash == 0);
}

TEST(geometry_two_solids) {
    std::vector<uint8_t> root;
    append_solid(root, 1, 0, vertex_floats_with_normal(), index_triangle());
    append_solid(root, 2, 0, vertex_floats_no_normal(), index_triangle());

    std::vector<GeomSolid> out;
    std::string err = parse_geometry(root.data(), root.size(), out);
    CHECK(err.empty());
    CHECK(out.size() == 2);
    CHECK(out[0].has_normal);
    CHECK(!out[1].has_normal);
}

// Unbekanntes Vertexformat (passt zu keiner bekannten Stride) wird übersprungen, nicht als
// Fehler gemeldet.
TEST(geometry_skips_unknown_format) {
    std::vector<uint8_t> root;
    std::vector<uint8_t> vbuf(10, 0);  // 10 Byte passt zu keiner bekannten Stride (24/36)
    std::vector<uint8_t> ibuf;
    put16(ibuf, 0); put16(ibuf, 1);
    append_solid(root, 1, 0, vbuf, ibuf);

    std::vector<GeomSolid> out;
    size_t skipped = 0;
    std::string err = parse_geometry(root.data(), root.size(), out, &skipped);
    CHECK(err.empty());
    CHECK(out.empty());
    CHECK(skipped == 1);
}
