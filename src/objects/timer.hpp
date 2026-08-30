#pragma once

#include <cstdint>
#include "base/font.hpp"
#include "base/game_context.hpp"
#include "base/object_base.hpp"
#include "base/text.hpp"

class Timer : public ObjectBase
{
  private:
    uint64_t timer_start{0};
    uint64_t timer_paused{0};
    bool started{false};
    bool paused{false};
    Text text;
    SDL_FPoint position{.x = 10.F, .y = 10.F};

  public:
    Timer(const Timer &) = delete;
    Timer(Timer &&) noexcept = delete;
    Timer &operator=(const Timer &) = delete;
    Timer &operator=(Timer &&) noexcept = delete;

    Timer(GameContext &context, Font &font) : Timer(context, font, {.x = 10.F, .y = 10.F}) {};
    Timer(GameContext &context, Font &font, SDL_FPoint pos);
    ~Timer() override = default;

    void update(float delta_t) override;
    void render(SDL_Renderer *renderer) override;

    void start();
    void stop();
    void pause();
    void unpause();
    [[nodiscard]] bool isStarted() const;
    [[nodiscard]] bool isPaused() const;
    [[nodiscard]] uint64_t getTicksNS() const;
};
