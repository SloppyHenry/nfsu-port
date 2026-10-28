#include <cstdint>
#include <cstring>
#include <vector>

#include "formats/ea_audio.hpp"
#include "test.hpp"

static void tag(std::vector<uint8_t>& v, const char* t, const std::vector<uint8_t>& body) {
    v.insert(v.end(), t, t + 4);
    uint32_t n = uint32_t(body.size() + 8);
    for (int i = 0; i < 4; ++i) v.push_back(uint8_t(n >> (8 * i)));
    v.insert(v.end(), body.begin(), body.end());
}

// Ein Strom: mono, 1 Block mit Rohframe (0xEE) und einem Nullframe.
static std::vector<uint8_t> make_stream() {
    std::vector<uint8_t> s;
    tag(s, "SCHl", {'P', 'T', 0, 0, 0xFD, 0x80, 1, 2, 0x85, 1, 56, 0xFF});
    tag(s, "SCCl", {1, 0, 0, 0});
    std::vector<uint8_t> blk = {56, 0, 0, 0, 0, 0, 0, 0};            // 56 Samples, Offset Kanal 0 = 0
    blk.push_back(0xEE); blk.insert(blk.end(), {0, 10, 0, 5});       // cur=10, prev=5
    for (int i = 0; i < 28; ++i) { blk.push_back(0); blk.push_back(uint8_t(i)); }
    blk.push_back(0x00);                                             // Koeffizient 0, Shift 20
    for (int i = 0; i < 14; ++i) blk.push_back(0);
    tag(s, "SCDl", blk);
    tag(s, "SCEl", {});
    return s;
}

TEST(ea_stream_raw_and_zero_frame) {
    auto s = make_stream();
    std::vector<nfsu::ea::Stream> ss;
    CHECK(nfsu::ea::scan_streams(s.data(), s.size(), ss).empty());
    CHECK(ss.size() == 1);
    CHECK(ss[0].hdr.sample_rate == 22050 && ss[0].hdr.channels == 1 && ss[0].hdr.num_samples == 56);
    std::vector<int16_t> pcm;
    CHECK(nfsu::ea::decode_stream(s.data(), s.size(), ss[0], pcm).empty());
    CHECK(pcm.size() == 56);
    for (int i = 0; i < 28; ++i) CHECK(pcm[i] == i);
    for (int i = 28; i < 56; ++i) CHECK(pcm[i] == 0);   // Koeffizienten 0, Nibbles 0
}

TEST(ea_stream_rejects_truncated_block) {
    auto s = make_stream();
    s.resize(s.size() - 30);
    std::vector<nfsu::ea::Stream> ss;
    CHECK(!nfsu::ea::scan_streams(s.data(), s.size(), ss).empty());
}

TEST(wav_header) {
    int16_t pcm[2] = {1, -2};
    auto w = nfsu::ea::make_wav(pcm, 2, 1, 22050);
    CHECK(w.size() == 48);
    CHECK(!std::memcmp(w.data(), "RIFF", 4) && !std::memcmp(w.data() + 8, "WAVEfmt ", 8));
    CHECK(w[44] == 1 && w[46] == 0xFE && w[47] == 0xFF);
}
