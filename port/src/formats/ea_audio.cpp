#include "formats/ea_audio.hpp"

#include <cstring>

namespace nfsu::ea {
namespace {

uint32_t rd32(const uint8_t* p) { return p[0] | p[1] << 8 | p[2] << 16 | uint32_t(p[3]) << 24; }
int16_t be16(const uint8_t* p) { return int16_t(p[0] << 8 | p[1]); }

constexpr int kTable[20] = {0, 240, 460, 392, 0, 0, -208, -220, 0, 1, 3, 4, 7, 8, 10, 11, 0, -1, -3, -4};

int16_t clip16(int v) { return v < -32768 ? -32768 : v > 32767 ? 32767 : int16_t(v); }

bool rd_var(const uint8_t* b, size_t len, size_t& p, uint32_t& v) {
    if (p >= len) return false;
    size_t n = b[p++];
    if (n > 4 || p + n > len) return false;
    v = 0;
    for (size_t i = 0; i < n; ++i) v = v << 8 | b[p++];
    return true;
}

bool parse_header(const uint8_t* b, size_t len, StreamHeader& h, uint32_t* tags = nullptr) {
    // b = Nutzlast des SCHl: "PT\0\0" (Plattform), dann Elemente bis 0xFF
    if (len < 4) return false;
    size_t p = 4;
    bool sub = false;
    while (p < len) {
        uint8_t t = b[p++];
        if (t == 0xFF) return true;
        if (t == 0xFD) { sub = true; continue; }
        uint32_t v;
        if (!rd_var(b, len, p, v)) return false;
        if (tags) {
            switch (t) {
                case 0x86: tags[0] = v; break;   // Schleifenanfang
                case 0x87: tags[1] = v; break;   // Schleifenende
                case 0x88: tags[2] = v; break;   // Datenoffset (BNK)
                case 0xA0: tags[3] = v; break;   // Codec 2 (BNK)
                default: break;
            }
        }
        if (!sub) continue;
        switch (t) {
            case 0x80: h.codec = v; break;
            case 0x82: h.channels = v; break;
            case 0x84: h.sample_rate = v; break;
            case 0x85: h.num_samples = v; break;
            default: break;
        }
    }
    return false;
}

struct ChState { int cur = 0, prev = 0; };

// Frames des EA-XA-R2-Stroms ab p[q]: 0xEE = Rohframe (neues cur/prev + 28 BE16-Samples, 61 Byte),
// sonst Koeffizientenbyte + 14 Byte Nibbles (15 Byte). Schreibt je Frame 28 Samples im Abstand stride.
bool decode_r2_frames(const uint8_t* p, size_t n, size_t& q, size_t frames, ChState& st, int16_t* o,
                      size_t stride) {
    int cur = st.cur, prev = st.prev;
    for (size_t f = 0; f < frames; ++f) {
        if (q >= n) return false;
        uint8_t byte = p[q++];
        if (byte == 0xEE) {
            if (q + 4 + 56 > n) return false;
            cur = be16(p + q); prev = be16(p + q + 2); q += 4;
            for (int i = 0; i < 28; ++i, q += 2, o += stride) *o = be16(p + q);
        } else {
            int c1 = kTable[byte >> 4], c2 = kTable[(byte >> 4) + 4], shift = 20 - (byte & 15);
            if (q + 14 > n) return false;
            for (int i = 0; i < 28; ++i, o += stride) {
                int nib;
                if (i & 1) nib = byte & 15;
                else byte = p[q++], nib = byte >> 4;
                nib = (nib ^ 8) - 8;
                int s = clip16((int(unsigned(nib) << shift) + cur * c1 + prev * c2) >> 8);
                prev = cur; cur = s;
                *o = int16_t(s);
            }
        }
    }
    st.cur = cur; st.prev = prev;
    return true;
}

// Ein SCDl-Block: u32 Samplezahl, je Kanal u32 Offset (ab Ende der Offsettabelle), danach je Kanal Frames.
bool decode_block(const uint8_t* p, size_t n, uint32_t ch, ChState* st, std::vector<int16_t>& pcm) {
    if (n < 4 + 4 * size_t(ch)) return false;
    uint32_t count = rd32(p) & 0x0FFFFFFFu;
    size_t frames = count / 28;
    size_t base = pcm.size();
    pcm.resize(base + frames * 28 * ch);   // unvollständiges Endframe wird (wie bei FFmpeg) verworfen
    for (uint32_t c = 0; c < ch; ++c) {
        size_t q = rd32(p + 4 + 4 * c) + (ch + 1) * 4;
        if (!decode_r2_frames(p, n, q, frames, st[c], pcm.data() + base + c, ch)) return false;
    }
    return true;
}

}  // namespace

std::string scan_streams(const uint8_t* buf, size_t len, std::vector<Stream>& out) {
    size_t p = 0;
    while (p < len) {
        if (rd32(buf + p) == 0) { ++p; while (p < len && buf[p] == 0) ++p; continue; }
        if (p + 8 > len || std::memcmp(buf + p, "SCHl", 4)) return "SCHl erwartet bei " + std::to_string(p);
        Stream s; s.offset = p;
        s.hdr.sample_rate = 22050;   // Standard, wenn Element 0x84 fehlt (Musik, Sprache)
        size_t q = p;
        bool end = false, have_hdr = false;
        while (q + 8 <= len && !end) {
            uint32_t sz = rd32(buf + q + 4);
            if (sz < 8 || q + sz > len) return "Chunkgroesse bei " + std::to_string(q);
            if (!std::memcmp(buf + q, "SCHl", 4)) {
                if (q != p || !parse_header(buf + q + 8, sz - 8, s.hdr)) return "Kopf bei " + std::to_string(q);
                have_hdr = true;
            } else if (!std::memcmp(buf + q, "SCDl", 4)) ++s.blocks;
            else if (!std::memcmp(buf + q, "SCEl", 4)) end = true;
            else if (std::memcmp(buf + q, "SCCl", 4)) return "unbekannter Chunk bei " + std::to_string(q);
            q += sz;
        }
        if (!have_hdr || !end) return "Strom ohne Ende bei " + std::to_string(p);
        s.size = q - p;
        out.push_back(s);
        p = q;
    }
    return {};
}

std::string decode_stream(const uint8_t* buf, size_t len, const Stream& s, std::vector<int16_t>& pcm) {
    if (s.hdr.codec != 2) return "Codec " + std::to_string(s.hdr.codec) + " nicht unterstuetzt";
    if (s.hdr.channels < 1 || s.hdr.channels > 6) return "Kanalzahl";
    ChState st[6];
    size_t q = s.offset, end = s.offset + s.size;
    if (end > len) return "Strom ausserhalb";
    while (q + 8 <= end) {
        uint32_t sz = rd32(buf + q + 4);
        if (!std::memcmp(buf + q, "SCDl", 4) &&
            !decode_block(buf + q + 8, sz - 8, s.hdr.channels, st, pcm))
            return "Block bei " + std::to_string(q);
        q += sz;
    }
    return {};
}

std::vector<uint8_t> make_wav(const int16_t* pcm, size_t frames, uint32_t channels, uint32_t rate) {
    uint32_t data = uint32_t(frames * channels * 2);
    std::vector<uint8_t> w(44 + data);
    auto w32 = [&](size_t o, uint32_t v) { for (int i = 0; i < 4; ++i) w[o + i] = uint8_t(v >> (8 * i)); };
    auto w16 = [&](size_t o, uint32_t v) { w[o] = uint8_t(v); w[o + 1] = uint8_t(v >> 8); };
    std::memcpy(&w[0], "RIFF", 4); w32(4, 36 + data);
    std::memcpy(&w[8], "WAVEfmt ", 8); w32(16, 16); w16(20, 1); w16(22, channels);
    w32(24, rate); w32(28, rate * channels * 2); w16(32, channels * 2); w16(34, 16);
    std::memcpy(&w[36], "data", 4); w32(40, data);
    for (size_t i = 0; i < frames * channels; ++i) w16(44 + 2 * i, uint16_t(pcm[i]));
    return w;
}

std::string parse_abk(const uint8_t* buf, size_t len, Bank& bank) {
    if (len < 0x40 || std::memcmp(buf, "ABKC", 4)) return "kein ABKC";
    bank.version = rd32(buf + 4);
    bank.file_size = rd32(buf + 0x14);
    bank.header_size = rd32(buf + 0x18);
    for (int a = 0; a < 2; ++a) { bank.area_off[a] = rd32(buf + 0x20 + 8 * a); bank.area_size[a] = rd32(buf + 0x24 + 8 * a); }
    for (int t = 0; t < 3; ++t) {
        uint32_t o = rd32(buf + 0x30 + 4 * t);
        if (size_t(o) + 4 > len) return "Tabelle ausserhalb";
        bank.table_count[t] = rd32(buf + o);
    }
    if (bank.file_size != len) return "Dateigroesse stimmt nicht mit Kopf";
    for (int a = 0; a < 2; ++a) {
        size_t base = bank.area_off[a], asz = bank.area_size[a];
        if (!asz) continue;
        if (base + asz > len || asz < 0x14 || std::memcmp(buf + base, "BNKl", 4)) return "BNKl erwartet";
        uint32_t cnt = buf[base + 6] | buf[base + 7] << 8;
        if (0x14 + 4 * size_t(cnt) > asz) return "BNK-Tabelle";
        std::vector<size_t> begin;
        for (uint32_t i = 0; i < cnt; ++i) {
            uint32_t rel = rd32(buf + base + 0x14 + 4 * i);
            if (!rel) continue;
            size_t p = base + 0x14 + 4 * i + rel;
            if (p + 6 > base + asz || std::memcmp(buf + p, "PT", 2)) return "Soundkopf";
            BnkSound s; s.area = a; s.index = i;
            uint32_t tg[4] = {0, 0, 0, 0xFFFFFFFFu};
            s.hdr.sample_rate = 22050;
            if (!parse_header(buf + p, base + asz - p, s.hdr, tg)) return "Soundkopf unlesbar";
            s.loop_start = tg[0]; s.loop_end = tg[1]; s.codec2 = tg[3];
            s.data_offset = base + tg[2];
            if (s.data_offset > base + asz) return "Datenoffset";
            bank.sounds.push_back(s);
        }
    }
    // Obergrenze der Daten je Sound: Beginn des nächsten Sounds derselben Fläche bzw. Flächenende
    for (auto& s : bank.sounds) {
        size_t lim = bank.area_off[s.area] + bank.area_size[s.area];
        for (auto& t : bank.sounds)
            if (t.area == s.area && t.data_offset > s.data_offset && t.data_offset < lim) lim = t.data_offset;
        s.data_limit = lim;
    }
    return {};
}

std::string decode_bnk_sound(const uint8_t* buf, size_t len, const BnkSound& s, std::vector<int16_t>& pcm,
                             size_t* used) {
    if (s.hdr.channels != 1) return "Kanalzahl";
    if (s.data_limit > len || s.data_offset > s.data_limit) return "Daten ausserhalb";
    size_t n = s.data_limit - s.data_offset, ns = s.hdr.num_samples;
    const uint8_t* p = buf + s.data_offset;
    if (s.codec2 == 8) {   // PCM16 little-endian
        if (ns * 2 > n) return "PCM ueberschreitet Grenze";
        pcm.resize(ns);
        for (size_t i = 0; i < ns; ++i) pcm[i] = int16_t(p[2 * i] | p[2 * i + 1] << 8);
        if (used) *used = ns * 2;
        return {};
    }
    if (s.codec2 != 0xFFFFFFFFu) return "Codec2 " + std::to_string(s.codec2) + " unbekannt";
    size_t frames = (ns + 27) / 28, q = 0;
    ChState st;
    pcm.resize(frames * 28);
    if (!decode_r2_frames(p, n, q, frames, st, pcm.data(), 1)) return "EA-XA-Daten enden zu frueh";
    pcm.resize(ns);
    if (used) *used = q;
    return {};
}

}  // namespace nfsu::ea
