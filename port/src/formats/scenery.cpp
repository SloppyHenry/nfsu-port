#include "formats/scenery.hpp"

#include <cstring>

#include "formats/chunk.hpp"

namespace nfsu {

namespace {

// GC LoaderScenery: (Chunkdaten + 15) & ~15, bezogen auf den Ladepuffer.
size_t align16(size_t off) { return (off + 15) & ~size_t(15); }

}  // namespace

// orig: GC 0x800447A0 LoaderScenery
std::string parse_scenery(const uint8_t* buf, size_t len, std::vector<ScenerySection>& out) {
    std::string err;
    std::string walk_err = walk_chunks(buf, len, [&](const ChunkView& c) {
        if (!err.empty()) return false;
        if (c.id != 0x80034100u) return c.container() && c.id != 0x80134000u && c.id != 0xb3300000u;
        ScenerySection sec;
        bool have_header = false;
        const uint8_t* end = c.data + c.size;
        for (const uint8_t* p = c.data; p + 8 <= end;) {
            uint32_t id, size;
            std::memcpy(&id, p, 4);
            std::memcpy(&size, p + 4, 4);
            const uint8_t* data = p + 8;
            if (data + size > end) { err = "ScenerySection: Kind über Containerende"; return false; }
            size_t base = size_t(data - buf);
            if (id == 0x34101) {
                const uint8_t* h = buf + align16(base);
                if (h + 0x10 > data + size) { err = "0x34101 zu kurz"; return false; }
                ScenerySectionHeaderDisk hdr{};
                size_t avail = size_t(data + size - h);
                std::memcpy(&hdr, h, avail < sizeof hdr ? avail : sizeof hdr);
                sec.number = hdr.section_number;
                have_header = true;
            } else if (id == 0x34102) {
                sec.infos.resize(size / sizeof(SceneryInfoDisk));
                std::memcpy(sec.infos.data(), data, sec.infos.size() * sizeof(SceneryInfoDisk));
            } else if (id == 0x34103) {
                size_t skip = align16(base) - base;
                if (skip > size) { err = "0x34103 zu kurz"; return false; }
                sec.instances.resize((size - skip) / sizeof(SceneryInstanceDisk));
                std::memcpy(sec.instances.data(), data + skip, sec.instances.size() * sizeof(SceneryInstanceDisk));
            }
            p = data + size;
        }
        if (!have_header) { err = "ScenerySection ohne 0x34101"; return false; }
        for (const auto& inst : sec.instances) {
            if (inst.scenery_info_number < 0 || size_t(inst.scenery_info_number) >= sec.infos.size()) {
                err = "SceneryInstance: Infoindex außerhalb";
                return false;
            }
        }
        out.push_back(std::move(sec));
        return false;
    });
    return walk_err.empty() ? err : walk_err;
}

// orig: 0x442880 (Rotation als 3×3 Shorts, Einheit 1/8192 = 0.00012207031)
void scenery_instance_matrix(const SceneryInstanceDisk& inst, float m[16]) {
    std::memset(m, 0, 16 * sizeof(float));
    m[15] = 1.0f;
    if (inst.exclude_flags & kSceneryExcludeIdentity) {
        m[0] = m[5] = m[10] = 1.0f;
        return;
    }
    for (int r = 0; r < 3; ++r)
        for (int c = 0; c < 3; ++c) m[r * 4 + c] = float(inst.rotation[r * 3 + c]) * (1.0f / 8192.0f);
    m[12] = inst.position[0];
    m[13] = inst.position[1];
    m[14] = inst.position[2];
}

// orig: GC 0x800446D4 GetScenerySectionName ("%c%d", '@' + n/100, n%100)
std::string scenery_section_name(int number) {
    if (number < 1) return "--";
    return std::string(1, char('@' + number / 100)) + std::to_string(number % 100);
}

}  // namespace nfsu
