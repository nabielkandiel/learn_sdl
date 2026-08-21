#include "utility.h"
#include <SDL3/SDL.h>
#include <string>
#include "SDL3/SDL_error.h"
#include "SDL3/SDL_init.h"
#include "SDL3/SDL_log.h"
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_video.h"
#include "config.h"

bool init(SDL_Window **window_pp, SDL_Renderer **renderer_pp, int width, int height)
{
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("SDL could not initialize - SDL error :%s\n", SDL_GetError());
        return false;
    }

    if (!SDL_CreateWindowAndRenderer("Nasty Tetris", width, height, 0, window_pp, renderer_pp)) {
        SDL_Log("Create Window and Renderer failed - SDL error: %s\n", SDL_GetError());
        return false;
    }

    if (!TTF_Init()) {
        SDL_Log("TTF Init failed - SDL error: %s\n", SDL_GetError());
        return false;
    }

    return true;
}

// loads media
bool loadAsset(Texture &sdl_texture, SDL_Renderer *sdl_renderer, std::string_view fname)
{
    std::string image_path{ASSET_PATH};
    image_path += fname;
    if (!sdl_texture.loadFromFile(image_path, sdl_renderer)) {
        SDL_Log("Unable to load image %s - SDL error: %s\n", image_path.c_str(), SDL_GetError());
        return false;
    }
    return true;
}

// frees media and shuts down SDL

void close(SDL_Window **window_pp, SDL_Renderer **renderer_pp)
{
    SDL_DestroyWindow(*window_pp);
    *window_pp = nullptr;
    SDL_DestroyRenderer(*renderer_pp);
    *renderer_pp = nullptr;
    TTF_Quit();
    SDL_Quit();
}
