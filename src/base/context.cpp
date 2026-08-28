#include "context.hpp"

bool Context::init()
{
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("SDL could not initialize - SDL error :%s\n", SDL_GetError());
        return false;
    }

    if (!SDL_CreateWindowAndRenderer("Nasty Tetris", width, height, 0, &window, &renderer)) {
        SDL_Log("Create Window and Renderer failed - SDL error: %s\n", SDL_GetError());
        return false;
    }

    if (!TTF_Init()) {
        SDL_Log("TTF Init failed - SDL error: %s\n", SDL_GetError());
        return false;
    }

    return true;
}

void Context::close()
{
    SDL_DestroyWindow(window);
    window = nullptr;
    SDL_DestroyRenderer(renderer);
    renderer = nullptr;
    TTF_Quit();
    SDL_Quit();
}