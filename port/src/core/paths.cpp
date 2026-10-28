#include "core/paths.hpp"

#include <algorithm>
#include <cstdlib>
#include <system_error>

namespace fs = std::filesystem;

namespace nfsu {

std::string to_lower(std::string_view s) {
    std::string r(s);
    for (char& c : r)
        if (c >= 'A' && c <= 'Z') c = static_cast<char>(c - 'A' + 'a');
    return r;
}

bool wildcard_match(std::string_view p, std::string_view t) {
    size_t pi = 0, ti = 0, star = std::string_view::npos, mark = 0;
    auto eq = [](char a, char b) { return to_lower(std::string_view(&a, 1)) == to_lower(std::string_view(&b, 1)); };
    while (ti < t.size()) {
        if (pi < p.size() && (p[pi] == '?' || (p[pi] != '*' && eq(p[pi], t[ti])))) {
            ++pi; ++ti;
        } else if (pi < p.size() && p[pi] == '*') {
            star = pi++; mark = ti;
        } else if (star != std::string_view::npos) {
            pi = star + 1; ti = ++mark;
        } else {
            return false;
        }
    }
    while (pi < p.size() && p[pi] == '*') ++pi;
    return pi == p.size();
}

fs::path find_data_root(const char* cli_arg) {
    if (cli_arg && *cli_arg) return cli_arg;
    if (const char* env = std::getenv("NFSU_DATA"); env && *env) return env;
    return NFSU_DEFAULT_DATA;
}

DataPaths::DataPaths(fs::path root) : root_(std::move(root)) {}

const DataPaths::DirMap& DataPaths::listing(const fs::path& dir) {
    auto key = dir.string();
    auto it = dir_cache_.find(key);
    if (it != dir_cache_.end()) return it->second;
    DirMap map;
    std::error_code ec;
    for (fs::directory_iterator d(dir, ec), end; !ec && d != end; d.increment(ec)) {
        std::string name = d->path().filename().string();
        map.emplace(to_lower(name), name);  // bei Kollision gewinnt der erste Eintrag
    }
    return dir_cache_.emplace(std::move(key), std::move(map)).first->second;
}

static std::vector<std::string> split_segments(std::string_view path) {
    std::vector<std::string> out;
    std::string cur;
    for (char c : path) {
        if (c == '\\' || c == '/') {
            if (!cur.empty()) out.push_back(std::move(cur));
            cur.clear();
        } else {
            cur.push_back(c);
        }
    }
    if (!cur.empty()) out.push_back(std::move(cur));
    return out;
}

std::optional<fs::path> DataPaths::resolve(std::string_view game_path) {
    fs::path cur = root_;
    for (const auto& seg : split_segments(game_path)) {
        if (seg == ".") continue;
        if (seg == "..") return std::nullopt;  // kein Ausbruch aus dem Datenordner
        const DirMap& m = listing(cur);
        auto it = m.find(to_lower(seg));
        if (it == m.end()) return std::nullopt;
        cur /= it->second;
    }
    return cur;
}

std::vector<fs::path> DataPaths::glob(std::string_view pattern) {
    auto segs = split_segments(pattern);
    std::vector<fs::path> dirs{root_};
    for (size_t i = 0; i < segs.size(); ++i) {
        std::vector<fs::path> next;
        bool wild = segs[i].find_first_of("*?") != std::string::npos;
        for (const auto& d : dirs) {
            const DirMap& m = listing(d);
            if (!wild) {
                if (auto it = m.find(to_lower(segs[i])); it != m.end()) next.push_back(d / it->second);
            } else {
                for (const auto& [low, name] : m)
                    if (wildcard_match(segs[i], name)) next.push_back(d / name);
            }
        }
        dirs = std::move(next);
    }
    std::sort(dirs.begin(), dirs.end());
    return dirs;
}

}  // namespace nfsu
