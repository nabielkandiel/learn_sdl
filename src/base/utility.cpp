#include "utility.h"
#include <SDL3/SDL.h>
#include <string>
#include "SDL3/SDL_error.h"
#include "SDL3/SDL_log.h"
#include "SDL3/SDL_render.h"
#include "config.h"

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
