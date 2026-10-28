#include "formats/chunk.hpp"

#include <cstring>

#include "formats/jdlz.hpp"

namespace nfsu {

static uint32_t rd32(const uint8_t* p) { uint32_t v; std::memcpy(&v, p, 4); return v; }

static std::string walk(const uint8_t* root, size_t begin, size_t end, int depth,
                        const std::function<bool(const ChunkView&)>& visit) {
    size_t pos = begin;
    while (pos < end) {
        if (end - pos < 8) return "Kopf ragt ueber das Ende bei " + std::to_string(pos);
        ChunkView c;
        c.id = rd32(root + pos);
        c.size = rd32(root + pos + 4);
        c.data = root + pos + 8;
        c.depth = depth;
        c.offset = pos;
        if (c.size > end - pos - 8)
            return "Chunk " + std::to_string(c.id) + " bei " + std::to_string(pos) + " zu gross";
        bool descend = visit(c);
        if (c.container() && descend) {
            std::string err = walk(root, pos + 8, pos + 8 + c.size, depth + 1, visit);
            if (!err.empty()) return err;
        }
        pos += 8 + static_cast<size_t>(c.size);
    }
    return {};
}

std::string walk_chunks(const uint8_t* buf, size_t len,
                        const std::function<bool(const ChunkView&)>& visit) {
    return walk(buf, 0, len, 0, visit);
}

std::vector<uint8_t> unpack_lzc(const uint8_t* src, size_t len) {
    if (len < 0x12 || rd32(src) != 0x5a4c444au || src[4] != 2) return {};
    const uint32_t out_size = rd32(src + 8);
    if (rd32(src + 12) > len) return {};
    std::vector<uint8_t> out(out_size);
    if (jdlz_decompress(src, out.data()) != out_size) return {};
    return out;
}

}  // namespace nfsu
