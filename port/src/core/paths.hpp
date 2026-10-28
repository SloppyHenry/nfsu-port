// Auflösung von Spieldatenpfaden: Groß-/Kleinschreibung egal, '\' wird zu '/'.
#pragma once
#include <filesystem>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace nfsu {

class DataPaths {
public:
    explicit DataPaths(std::filesystem::path root);

    const std::filesystem::path& root() const { return root_; }

    // Löst einen Spielpfad (z. B. "TRACKS\\TRACKB1001.LZC") zur echten Datei auf.
    std::optional<std::filesystem::path> resolve(std::string_view game_path);

    // Wie resolve, aber mit Platzhaltern '*' und '?' im letzten Segment. Sortiert.
    std::vector<std::filesystem::path> glob(std::string_view pattern);

    void clear_cache() { dir_cache_.clear(); }

private:
    // Kleinschreibung -> echter Name, je Verzeichnis einmal eingelesen.
    using DirMap = std::unordered_map<std::string, std::string>;
    const DirMap& listing(const std::filesystem::path& dir);

    std::filesystem::path root_;
    std::unordered_map<std::string, DirMap> dir_cache_;
};

// Reihenfolge: cli_arg, Umgebungsvariable NFSU_DATA, Standard <repo>/game/pc.
std::filesystem::path find_data_root(const char* cli_arg);

std::string to_lower(std::string_view s);
bool wildcard_match(std::string_view pattern, std::string_view text);  // ohne Beachtung der Schreibweise

}  // namespace nfsu
