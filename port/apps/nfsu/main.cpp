#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "core/paths.hpp"
#include "platform/window.hpp"

int main(int argc, char** argv) {
    const char* data_arg = nullptr;
    int frames = 0;
    for (int i = 1; i < argc; ++i) {
        if (!std::strcmp(argv[i], "--data") && i + 1 < argc) data_arg = argv[++i];
        else if (!std::strcmp(argv[i], "--frames") && i + 1 < argc) frames = std::atoi(argv[++i]);
    }
    std::printf("Spieldaten: %s\n", nfsu::find_data_root(data_arg).c_str());
    return nfsu::run_clear_window(frames);
}
