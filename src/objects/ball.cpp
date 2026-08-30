#include "ball.hpp"

Ball::Ball(GameContext &context, SDL_FPoint bounds)
    : sprite(context.getTexture(GameContext::TEXTURES::BALL),
             {ballDir::LEFT, ballDir::UP, ballDir::DOWN, ballDir::RIGHT}),
      maxBounds(bounds)
{
    makeCenter();
    setupSprite(32.0F, 32.0F, {.rows = 2, .cols = 2});

    auto &input_manager = context.getInputManager();
    input_manager.bindKeyDown(SDLK_UP, [this]() { setDirection(ballDir::UP); });
    input_manager.bindKeyDown(SDLK_DOWN, [this]() { setDirection(ballDir::DOWN); });
    input_manager.bindKeyDown(SDLK_LEFT, [this]() { setDirection(ballDir::LEFT); });
    input_manager.bindKeyDown(SDLK_RIGHT, [this]() { setDirection(ballDir::RIGHT); });

    input_manager.bindHeldKey(SDL_SCANCODE_UP, [this](float delta_t) { tryUp(delta_t); });
    input_manager.bindHeldKey(SDL_SCANCODE_DOWN, [this](float delta_t) { tryDown(delta_t); });
    input_manager.bindHeldKey(SDL_SCANCODE_LEFT, [this](float delta_t) { tryLeft(delta_t); });
    input_manager.bindHeldKey(SDL_SCANCODE_RIGHT, [this](float delta_t) { tryRight(delta_t); });

    context.getEntityManager().registerEntity(*this);
}

void Ball::update(float delat_t)
{
    if (!x_mov) {
        if (x_vel > 0.0F) {
            x_vel = std::max(x_vel - (FRICTION * delat_t), 0.0F);
        } else if (x_vel < 0.0F) {
            x_vel = std::min(x_vel + (FRICTION * delat_t), 0.0F);
        }
    }
    if (!y_mov) {
        if (y_vel > 0.0F) {
            y_vel = std::max(y_vel - (FRICTION * delat_t), 0.0F);
        } else if (y_vel < 0.0F) {
            y_vel = std::min(y_vel + (FRICTION * delat_t), 0.0F);
        }
    }

    position.x += x_vel * delat_t;
    position.x = std::clamp(position.x, 0.0F, maxBounds.x);
    x_mov = false;
    position.y += y_vel * delat_t;
    position.y = std::clamp(position.y, 0.0F, maxBounds.y);
    y_mov = false;
}