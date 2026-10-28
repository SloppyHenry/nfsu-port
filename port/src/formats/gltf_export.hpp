// Schreibt eSolid-Geometrie (geometry.hpp) als glTF 2.0 (Text + externes .bin). Reine
// Exportschicht für nfsu-tool mesh, kein Teil des Spielformats.
#pragma once
#include <functional>
#include <string>
#include <vector>

#include "formats/geometry.hpp"

namespace nfsu {

// Liefert die PNG-Bytes einer Textur zu ihrem name_hash, leer wenn nicht gefunden. Ohne
// Lookup (oder bei leerem Ergebnis) bekommt das Primitiv nur Vertexfarbe (COLOR_0), kein Bild.
using GltfTextureLookup = std::function<std::vector<uint8_t>(uint32_t texture_hash)>;

// Schreibt <path> (.gltf) und <path ohne .gltf>.bin daneben. Ein Primitiv je Solid.
// Leer bei Erfolg, sonst Fehlertext.
std::string write_gltf(const std::string& path, const std::vector<GeomSolid>& solids,
                       const GltfTextureLookup& texture_png = {});

}  // namespace nfsu
