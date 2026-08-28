#pragma once

#include <SDL3/SDL.h>
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "utility.h"

struct Context
{
    Context(const Context &) = default;
    Context(Context &&) = delete;
    Context &operator=(const Context &) = default;
    Context &operator=(Context &&) = delete;

    Context(Size2D screen) : width(screen.width), height(screen.height)
    {
        init();
    }

    ~Context()
    {
        close();
    }

    SDL_Window *window{nullptr};
    SDL_Renderer *renderer{nullptr};
    int width;
    int height;

    bool init();

    void close();
};