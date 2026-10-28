#include "platform/window.hpp"

#include <SDL3/SDL.h>

#include <cstdio>

namespace nfsu {

int run_clear_window(int frames) {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::fprintf(stderr, "SDL_Init: %s\n", SDL_GetError());
        return 1;
    }
    SDL_Window* window = SDL_CreateWindow("Need for Speed Underground", 800, 600, SDL_WINDOW_RESIZABLE);
    if (!window) {
        std::fprintf(stderr, "SDL_CreateWindow: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    SDL_GPUDevice* gpu = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_MSL, true, nullptr);
    if (!gpu || !SDL_ClaimWindowForGPUDevice(gpu, window)) {
        std::fprintf(stderr, "SDL_GPU: %s\n", SDL_GetError());
        if (gpu) SDL_DestroyGPUDevice(gpu);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    std::printf("GPU-Treiber: %s\n", SDL_GetGPUDeviceDriver(gpu));

    bool running = true;
    int frame = 0;
    while (running) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) running = false;
            if (e.type == SDL_EVENT_KEY_DOWN && e.key.key == SDLK_ESCAPE) running = false;
        }
        SDL_GPUCommandBuffer* cmd = SDL_AcquireGPUCommandBuffer(gpu);
        SDL_GPUTexture* swap = nullptr;
        if (cmd && SDL_WaitAndAcquireGPUSwapchainTexture(cmd, window, &swap, nullptr, nullptr) && swap) {
            SDL_GPUColorTargetInfo target{};
            target.texture = swap;
            target.clear_color = {0.05f, 0.07f, 0.12f, 1.0f};
            target.load_op = SDL_GPU_LOADOP_CLEAR;
            target.store_op = SDL_GPU_STOREOP_STORE;
            SDL_GPURenderPass* pass = SDL_BeginGPURenderPass(cmd, &target, 1, nullptr);
            SDL_EndGPURenderPass(pass);
        }
        if (cmd) SDL_SubmitGPUCommandBuffer(cmd);
        if (frames > 0 && ++frame >= frames) running = false;
    }

    SDL_WaitForGPUIdle(gpu);
    SDL_ReleaseWindowFromGPUDevice(gpu, window);
    SDL_DestroyGPUDevice(gpu);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

}  // namespace nfsu
