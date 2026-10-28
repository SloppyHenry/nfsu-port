// Minimaler PNG-Schreiber (RGBA8, ein IDAT, Kompression über die zlib des Systems).
#pragma once
#include <cstdint>
#include <string>
#include <vector>

namespace nfsu {

std::vector<uint8_t> encode_png_rgba(const uint8_t* rgba, int width, int height);
bool write_png_rgba(const std::string& path, const uint8_t* rgba, int width, int height);

}  // namespace nfsu
