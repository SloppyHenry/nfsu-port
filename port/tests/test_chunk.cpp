#include <cstdint>
#include <cstring>
#include <vector>

#include "formats/chunk.hpp"
#include "test.hpp"

static void put(std::vector<uint8_t>& v, uint32_t x) {
    uint8_t b[4]; std::memcpy(b, &x, 4); v.insert(v.end(), b, b + 4);
}

TEST(chunk_walk_nested) {
    std::vector<uint8_t> v;
    put(v, 0x80000001u); put(v, 8 + 4);       // Container mit einem Blatt
    put(v, 0x33310001u); put(v, 4); put(v, 7);
    put(v, 0); put(v, 0);                     // Füllchunk
    int n = 0;
    std::string err = nfsu::walk_chunks(v.data(), v.size(), [&](const nfsu::ChunkView& c) {
        ++n; (void)c; return true; });
    CHECK(err.empty());
    CHECK(n == 3);
}

TEST(chunk_walk_rejects_overrun) {
    std::vector<uint8_t> v;
    put(v, 0x33310001u); put(v, 100);
    CHECK(!nfsu::walk_chunks(v.data(), v.size(), [](const nfsu::ChunkView&) { return true; }).empty());
}

TEST(lzc_rejects_bad_header) {
    uint8_t junk[32] = {};
    CHECK(nfsu::unpack_lzc(junk, sizeof junk).empty());
}
