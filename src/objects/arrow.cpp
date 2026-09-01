#include "arrow.hpp"
#include <cmath>

Arrow::Arrow(GameContext &context, std::optional<SDL_FPoint> pos, Ball &ball)
    : ObjectBase(context), ball(ball), texture(context.getTexture(GameContext::TEXTURES::ARROW))
{
    if (pos) {
        position = pos.value();
    } else {
        position = {.x = 0.0F, .y = static_cast<float>(context.getScreenHeight() - texture.getHeight())};
    }

    center.x = position.x + (static_cast<float>(texture.getWidth()) / 2.0F);
    center.y = position.y + (static_cast<float>(texture.getHeight()) / 2.0F);
}

void Arrow::update(float /*unused*/)
{
    const SDL_FPoint ball_pos = ball.getPosition();
    const float dxx = ball_pos.x - center.x;
    const float dyy = ball_pos.y - center.y;
    degrees = std::atan2(dxx, -dyy) * (180.0 / std::numbers::pi);
}

void Arrow::render(SDL_Renderer *renderer)
{
    texture.renderWithTransform(position, renderer, nullptr, nullptr, degrees, nullptr, SDL_FLIP_NONE);
}