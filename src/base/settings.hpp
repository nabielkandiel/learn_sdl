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
    bool toggleVsync();

    void setFpsLimit(uint32_t fps)
    {
        fpsCap = fps;
        if (fps != 0U) {
            nsPerFrame = 1'000'000'000 / fps;
        }
    }

    [[nodiscard]] uint32_t fpsLimit() const
    {
        return fpsCap;
    }

    [[nodiscard]] uint64_t frameTimeNS() const
    {
        return nsPerFrame;
    }

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
    uint32_t fpsCap{0};
    uint64_t nsPerFrame{0};
    SDL_Renderer *renderer{nullptr};
};