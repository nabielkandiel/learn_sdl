#pragma once

#include <SDL3/SDL.h>

#include "base/collidable_base.hpp"
#include "base/game_context.hpp"
#include "base/object_base.hpp"
#include "base/sprites.hpp"

#include <algorithm>

class Ball : public ObjectBase, public CollidableBase
{
  private:
    enum class ballDir : uint8_t
    {
        UP = 0,
        DOWN = 1,
        LEFT = 2,
        RIGHT = 3,
        COUNT = 4
    };

    static constexpr size_t TOTAL_DIRS = static_cast<size_t>(ballDir::COUNT);
    Sprites<ballDir> sprite;

    SDL_FPoint position{.x = 0.0, .y = 0.0};
    SDL_FPoint maxBounds;
    float x_vel = 0;
    float y_vel = 0;
    bool x_mov{false};
    bool y_mov{false};

    static constexpr float ACCEL =
        2500.0F; // pixels/sec² — how fast it speeds up while held
    static constexpr float FRICTION =
        4000.0F; // pixels/sec² — how fast it slows when released
    static constexpr float MAX_SPEED =
        500.0F; // pixels/sec — cap so it doesn't accelerate forever

    void setupSprite(float spr_w, float spr_h, GridDimensions dims)
    {
        sprite.sliceTextureBox(spr_w, spr_h, dims);
        sprite.setActiveDir(ballDir::UP);
    }

  public:
    Ball(const Ball &) = delete;
    Ball(Ball &&) = delete;
    Ball &operator=(const Ball &) = delete;
    Ball &operator=(Ball &&) = delete;

    Ball(GameContext &context, SDL_FPoint bounds, bool use_arrow);
    ~Ball() override;

    void makeCenter()
    {
        position.x = maxBounds.x / 2.F;
        position.y = maxBounds.y / 2.F;
    }

    void setDirection(ballDir dir)
    {
        sprite.setActiveDir(dir);
    }

    void tryUp(float Sdt)
    {
        y_vel = std::max((y_vel - (ACCEL * Sdt)), -MAX_SPEED);
        y_mov = true;
    }

    void tryDown(float Sdt)
    {
        y_vel = std::min((y_vel + (ACCEL * Sdt)), MAX_SPEED);
        y_mov = true;
    }

    void tryLeft(float Sdt)
    {
        x_vel = std::max((x_vel - (ACCEL * Sdt)), -MAX_SPEED);
        x_mov = true;
    }

    void tryRight(float Sdt)
    {
        x_vel = std::min((x_vel + (ACCEL * Sdt)), MAX_SPEED);
        x_mov = true;
    }

    void render(SDL_Renderer *renderer) override
    {
        sprite.renderActive(renderer, position);
    }

    [[nodiscard]] SDL_FPoint getPosition() const
    {
        return position;
    }

    [[nodiscard]] Collider getCollider() const override
    {
        return {
            .type = ShapeType::CIRCLE,
            .circle = {.center_pos = position,
                       .radius = sprite.getActiveRect().w / 2.F},
        };
    }

    [[nodiscard]] float getInverseMass() const override
    {
        return 1.F;
    }

    [[nodiscard]] SDL_FPoint getVelocity() const override
    {
        return {.x = x_vel, .y = y_vel};
    }

    void setVelocity(SDL_FPoint new_vel) override
    {
        x_vel = new_vel.x;
        y_vel = new_vel.y;
    }

    void translate(SDL_FPoint delta) override
    {
        position.x = std::clamp(position.x + delta.x, 0.0F, maxBounds.x);
        position.y = std::clamp(position.y + delta.y, 0.0F, maxBounds.y);
    }

    void update(float delat_t) override;
};