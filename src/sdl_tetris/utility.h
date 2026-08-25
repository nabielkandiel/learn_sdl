#pragma once

#include <SDL3/SDL.h>
#include "texture.hpp"

// loads media
bool loadAsset(Texture &sdl_texture, SDL_Renderer *renderer, std::string_view fname);

struct GridDimensions
{
    size_t rows;
    size_t cols;
};

struct Size2D
{
    int width;
    int height;
};