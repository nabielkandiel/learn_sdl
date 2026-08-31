#pragma once

#include "base/font.hpp"
#include "base/game_context.hpp"
#include "base/object_base.hpp"
#include "base/text.hpp"
#include "base/timer.hpp"

class FrameLabel : public ObjectBase
{
  private:
    Text text;
    SDL_FPoint position{.x = 10.F, .y = 10.F};
    Timer timer;

  public:
    FrameLabel(const FrameLabel &) = delete;
    FrameLabel(FrameLabel &&) noexcept = delete;
    FrameLabel &operator=(const FrameLabel &) = delete;
    FrameLabel &operator=(FrameLabel &&) noexcept = delete;

    FrameLabel(GameContext &context, Font &font) : FrameLabel(context, font, {.x = 10.F, .y = 10.F}) {};
    FrameLabel(GameContext &context, Font &font, SDL_FPoint pos);
    ~FrameLabel() override = default;

    void update(float delta_t) override;
    void render(SDL_Renderer *renderer) override;
};
