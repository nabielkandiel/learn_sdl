#pragma once

#include <SDL3/SDL_render.h>

struct Settings
{
  public:
    Settings() = default;
    Settings(const Settings &) = delete;
    Settings(Settings &&) = delete;
    Settings &operator=(const Settings &) = delete;
    Settings &operator=(Settings &&) = delete;
    ~Settings() = default;

    bool enableVsync();
    bool disableVsync();

    [[nodiscard]] bool vsyncEnabled() const
    {
        return vSyncEnabled;
    }

    void setRenderer(SDL_Renderer *rend)
    {
        renderer = rend;
    }

  private:
    bool vSyncEnabled{false};
    bool fpsCapEnabled{false};
    SDL_Renderer *renderer{nullptr};
};