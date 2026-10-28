#include <cstdio>
#include <cstring>
#include <fstream>
#include <vector>

#include "core/ptr32.hpp"
#include "formats/texinfo.hpp"
#include "test.hpp"

using namespace nfsu;

TEST(ptr32_roundtrip) {
    std::vector<uint8_t> mem(4096);
    g_arena = {mem.data(), mem.size()};
    ptr32<uint32_t> p;
    CHECK(!p && p.get() == nullptr);
    auto* v = reinterpret_cast<uint32_t*>(mem.data() + 64);
    p = v;
    *p = 0xabcd;
    CHECK(p.off == 64 && p.get() == v && *v == 0xabcd);
    p = nullptr;
    CHECK(p.off == 0);
}

// Echter Chunk: TextureInfo-Tabelle aus Cars/240SX/TEXTURES.BIN. Ohne Spieldaten übersprungen.
TEST(texinfo_real_chunk) {
    std::ifstream f(std::string(NFSU_DEFAULT_DATA) + "/Cars/240SX/TEXTURES.BIN", std::ios::binary);
    if (!f) { std::printf("  (keine Spieldaten, übersprungen)\n"); return; }
    std::vector<uint8_t> d((std::istreambuf_iterator<char>(f)), {});
    g_arena = {d.data(), d.size()};  // Chunkpuffer ist selbst die Arena
    auto rd = [&](size_t o) { uint32_t v; std::memcpy(&v, &d[o], 4); return v; };
    const size_t o = 0xdb0;  // Chunk 0x33310004
    CHECK(rd(o) == 0x33310004);
    size_t n = rd(o + 4) / sizeof(TextureInfoDisk);
    CHECK(n == 0x39a4 / 0x7c && n * 0x7c == 0x39a4);
    auto* t = reinterpret_cast<TextureInfoDisk*>(&d[o + 8]);
    for (size_t i = 0; i < n; i++) {
        CHECK(!t[i].node_next && !t[i].node_prev && !t[i].plat_info);
        CHECK(!t[i].texture_pack && !t[i].image_data && !t[i].palette_data);
        CHECK(t[i].width > 0 && t[i].height > 0 && t[i].debug_name[0]);
    }
    // Fixup wie InternalLoadTexturePackHeaderChunks: Zeiger in der Arena setzen und lesen
    t[1].image_parent = &t[0];
    CHECK(t[1].image_parent->name_hash == t[0].name_hash);
}
