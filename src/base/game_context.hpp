#pragma once

#include <SDL3/SDL.h>
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "common/CommonEnums.hpp"

#include "entity_manager.hpp"
#include "input_manager.hpp"
#include "resource_manager.hpp"
#include "settings.hpp"
#include "utility.h"

class GameContext
{
  public:
    GameContext(const GameContext &) = delete;
    GameContext(GameContext &&) = delete;
    GameContext &operator=(const GameContext &) = delete;
    GameContext &operator=(GameContext &&) = delete;

    GameContext(Size2D screen);

    ~GameContext()
    {
        close();
    }

    State getState() const;
    void setState(State new_state);

    int getScreenWidth() const;
    int getScreenHeight() const;
    SDL_Renderer *getRenderer();
    InputManager &getInputManager();
    EntityManager &getEntityManager();
    ResourceManager &getResourceManager();
    Settings &getSettings();
    bool init();
    void close();

  private:
    SDL_Window *window{nullptr};
    SDL_Renderer *renderer{nullptr};
    InputManager inputManager;
    EntityManager entityManager;
    ResourceManager resourceManager;
    Settings settings;

    State state{State::PLAYING};

    int width;
    int height;
};