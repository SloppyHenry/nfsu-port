#include <filesystem>
#include <fstream>

#include "core/paths.hpp"
#include "test.hpp"

namespace fs = std::filesystem;

static fs::path make_tree() {
    fs::path root = fs::temp_directory_path() / "nfsu_test_paths";
    fs::remove_all(root);
    fs::create_directories(root / "Tracks");
    std::ofstream(root / "Tracks" / "TRACKB1001.lzc") << "x";
    std::ofstream(root / "Tracks" / "STREAML1RA.BUN") << "x";
    return root;
}

TEST(resolve_ignores_case_and_backslash) {
    fs::path root = make_tree();
    nfsu::DataPaths p(root);
    auto r = p.resolve("TRACKS\\TRACKB1001.LZC");
    CHECK(r.has_value());
    CHECK(r && r->filename() == "TRACKB1001.lzc");
    CHECK(!p.resolve("TRACKS\\NOPE.LZC").has_value());
    CHECK(!p.resolve("..\\etc").has_value());
}

TEST(glob_wildcards) {
    nfsu::DataPaths p(make_tree());
    CHECK(p.glob("tracks/*.lzc").size() == 1);
    CHECK(p.glob("Tracks\\*").size() == 2);
    CHECK(p.glob("Tracks/stream?1ra.bun").size() == 1);
}

TEST(wildcard_match_basic) {
    CHECK(nfsu::wildcard_match("*.LZC", "a.lzc"));
    CHECK(!nfsu::wildcard_match("a?c", "ac"));
}
