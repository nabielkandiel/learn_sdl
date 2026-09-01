#pragma once

#include <SDL3/SDL_render.h>
#include <cstdint>

#include "base/timer.hpp"
#include "game_context.hpp"

struct Settings
{
  public:
    Settings(GameContext &gameContext);
    Settings(const Settings &) = delete;
    Settings(Settings &&) = delete;
    Settings &operator=(const Settings &) = delete;
    Settings &operator=(Settings &&) = delete;
    ~Settings();

    void startTimer()
    {
        frame_timer.start();
    }
    void stopTimer()
    {
        frame_timer.stop();
    }

    bool enableVsync();
    bool disableVsync();

  private:
    bool vSyncEnabled{true};
    bool fpsCapEnabled{true};
    uint64_t renderingNs{0};
    SDL_Renderer *renderer{nullptr};
    Timer frame_timer;
    GameContext *context{nullptr};
};