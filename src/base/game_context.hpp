#pragma once

#include <SDL3/SDL.h>
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "entity_manager.hpp"
#include "input_manager.hpp"
#include "settings.hpp"
#include "utility.h"

class GameContext
{
  public:
    enum class TEXTURES : uint8_t
    {
        BALL = 0,
        ARROW = 1,
        RESET_BUTTON = 2,
    };

    GameContext(const GameContext &) = delete;
    GameContext(GameContext &&) = delete;
    GameContext &operator=(const GameContext &) = delete;
    GameContext &operator=(GameContext &&) = delete;

    GameContext(Size2D screen);

    ~GameContext()
    {
        close();
    }

    [[nodiscard]] int getScreenWidth() const
    {
        return width;
    }

    [[nodiscard]] int getScreenHeight() const
    {
        return height;
    }

    SDL_Renderer *getRenderer()
    {
        return renderer;
    }

    InputManager &getInputManager()
    {
        return inputManager;
    }

    EntityManager &getEntityManager()
    {
        return entityManager;
    }

    Settings &getSettings()
    {
        return settings;
    }

    Texture &getTexture(TEXTURES text)
    {
        return texture_map[text];
    }

    bool init();
    void close();

  private:
    void loadTextures();
    void destroyTextures();

    SDL_Window *window{nullptr};
    SDL_Renderer *renderer{nullptr};
    InputManager inputManager;
    EntityManager entityManager;
    Settings settings;
    std::unordered_map<TEXTURES, Texture> texture_map;

    int width;
    int height;
};