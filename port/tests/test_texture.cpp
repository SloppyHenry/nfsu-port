#include <cstring>
#include <fstream>
#include <vector>

#include "formats/chunk.hpp"
#include "formats/png.hpp"
#include "formats/texture_pack.hpp"
#include "test.hpp"

using namespace nfsu;

// DXT1-Block: c0 = Weiß, c1 = Schwarz, Indizes 0,1,2,3 je Zeile
TEST(dxt1_block_four_colors) {
    const uint8_t blk[8] = {0xff, 0xff, 0x00, 0x00, 0xe4, 0xe4, 0xe4, 0xe4};
    uint8_t px[4 * 4 * 4];
    decode_dxt1_block(blk, px, 16);
    CHECK(px[0] == 255 && px[3] == 255);           // Index 0 = c0
    CHECK(px[4] == 0 && px[7] == 255);             // Index 1 = c1
    CHECK(px[8] == 170 && px[12] == 85);           // 2/3, 1/3
}

// DXT1 mit c0 <= c1: Index 3 ist transparent
TEST(dxt1_block_transparent) {
    const uint8_t blk[8] = {0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
    uint8_t px[64];
    decode_dxt1_block(blk, px, 16);
    for (int p = 0; p < 16; ++p) CHECK(px[p * 4 + 3] == 0);
}

TEST(dxt3_alpha) {
    uint8_t blk[16] = {0x0f, 0x00};  // Pixel (0,0) Alpha 15 → 255, Rest 0
    uint8_t px[64];
    decode_dxt3_block(blk, px, 16);
    CHECK(px[3] == 255 && px[7] == 0 && px[63] == 0);
}

TEST(mip_sizes) {
    CHECK(mip_level_size(kD3DFmtDXT1, 128, 128) == 8192);
    CHECK(mip_level_size(kD3DFmtDXT3, 2, 1) == 16);
    CHECK(mip_level_size(kD3DFmtP8, 32, 16) == 512);
    CHECK(mip_level_size(kD3DFmtA8R8G8B8, 4, 4) == 64);
}

TEST(png_header) {
    uint8_t px[4] = {1, 2, 3, 4};
    auto png = encode_png_rgba(px, 1, 1);
    CHECK(png.size() > 33 && png[1] == 'P' && png[12] == 'I' && png[13] == 'H');
}

// Echtes Paket: alle Texturen eines Autos (Einzel-JDLZ) dekodieren. Ohne Spieldaten übersprungen.
TEST(texture_pack_real_car) {
    std::ifstream f(std::string(NFSU_DEFAULT_DATA) + "/Cars/240SX/TEXTURES.BIN", std::ios::binary);
    if (!f) { std::printf("  (keine Spieldaten, übersprungen)\n"); return; }
    std::vector<uint8_t> d((std::istreambuf_iterator<char>(f)), {});
    std::vector<TexturePackView> packs;
    CHECK(find_texture_packs(d.data(), d.size(), packs).empty());
    CHECK(packs.size() == 1 && packs[0].streams && packs[0].num_textures == 119);
    CHECK(packs[0].name() == "CARTEXTURES");
    CHECK(packs[0].header->filename_hash == 0xa5034852u);  // name_hash("Global\\Art\\Cars\\240SX\\240SX_Textures.tpk")
    std::vector<uint8_t> scratch, rgba;
    for (size_t k = 0; k < packs[0].num_textures; ++k) {
        TextureBytes tb;
        CHECK(texture_bytes(packs[0], k, scratch, tb).empty());
        CHECK(decode_texture_rgba(packs[0].infos[k], packs[0].plats[k].d3d_format, tb, rgba).empty());
    }
}
