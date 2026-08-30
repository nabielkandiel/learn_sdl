#pragma once

#include <SDL3/SDL.h>

#include "base/game_context.hpp"
#include "base/object_base.hpp"
#include "base/texture.hpp"

#include "ball.hpp"

class Arrow : public ObjectBase
{
  private:
    SDL_FPoint position{};
    SDL_FPoint center{};
    double degrees;
    Ball &ball;
    Texture &texture;

  public:
    Arrow(const Arrow &) = delete;
    Arrow(Arrow &&) noexcept = delete;
    Arrow &operator=(const Arrow &) = delete;
    Arrow &operator=(Arrow &&) noexcept = delete;

    Arrow(GameContext &context, std::optional<SDL_FPoint> position, Ball &ball);
    ~Arrow() override = default;

    void update(float /*delat_t*/) override;
    void render(SDL_Renderer *renderer) override;
};