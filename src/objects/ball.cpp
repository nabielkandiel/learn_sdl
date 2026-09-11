#include "ball.hpp"

#include "common/CommonEnums.hpp"

Ball::Ball(GameContext &context, SDL_FPoint bounds, bool use_arrow)
    : ObjectBase(context), CollidableBase(context.getCollisionManager()),
      sprite(context.getResourceManager().getTexture(Textures::BALL),
             {ballDir::LEFT, ballDir::UP, ballDir::DOWN, ballDir::RIGHT}),
      maxBounds(bounds)
{
    makeCenter();
    setupSprite(32.0F, 32.0F, {.rows = 2, .cols = 2});

    auto &input_manager = context.getInputManager();
    input_manager.bindKeyDown(this, use_arrow ? SDLK_UP : SDLK_W,
                              InputState::PLAYING,
                              [this]() { setDirection(ballDir::UP); });
    input_manager.bindKeyDown(this, use_arrow ? SDLK_DOWN : SDLK_S,
                              InputState::PLAYING,
                              [this]() { setDirection(ballDir::DOWN); });
    input_manager.bindKeyDown(this, use_arrow ? SDLK_LEFT : SDLK_A,
                              InputState::PLAYING,
                              [this]() { setDirection(ballDir::LEFT); });
    input_manager.bindKeyDown(this, use_arrow ? SDLK_RIGHT : SDLK_D,
                              InputState::PLAYING,
                              [this]() { setDirection(ballDir::RIGHT); });

    input_manager.bindHeldKey(
        this, use_arrow ? SDL_SCANCODE_UP : SDL_SCANCODE_W, InputState::PLAYING,
        [this](float delta_t) { tryUp(delta_t); });
    input_manager.bindHeldKey(
        this, use_arrow ? SDL_SCANCODE_DOWN : SDL_SCANCODE_S,
        InputState::PLAYING, [this](float delta_t) { tryDown(delta_t); });
    input_manager.bindHeldKey(
        this, use_arrow ? SDL_SCANCODE_LEFT : SDL_SCANCODE_A,
        InputState::PLAYING, [this](float delta_t) { tryLeft(delta_t); });
    input_manager.bindHeldKey(
        this, use_arrow ? SDL_SCANCODE_RIGHT : SDL_SCANCODE_D,
        InputState::PLAYING, [this](float delta_t) { tryRight(delta_t); });
}

Ball::~Ball()
{
    getInputManager().unbindAll(this);
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