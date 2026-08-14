#pragma once

#include <SDL3/SDL.h>

#include "../sprites.hpp"

#include <algorithm>

template <typename DirType> class Ball
{
  private:
    static constexpr size_t TOTAL_DIRS = static_cast<size_t>(DirType::COUNT);
    Sprites<DirType> sprite;

    SDL_FPoint position{.x = 0.0, .y = 0.0};
    SDL_FPoint maxBounds;
    float x_vel = 0;
    float y_vel = 0;
    bool x_mov{false};
    bool y_mov{false};

    static constexpr float ACCEL = 2500.0F;    // pixels/sec² — how fast it speeds up while held
    static constexpr float FRICTION = 4000.0F; // pixels/sec² — how fast it slows when released
    static constexpr float MAX_SPEED = 500.0F; // pixels/sec — cap so it doesn't accelerate forever

  public:
    Ball(SDL_FPoint bounds, std::array<DirType, TOTAL_DIRS> dir_map, std::optional<SDL_Color> color_key = std::nullopt)
        : sprite(dir_map, color_key.value_or(SDL_Color{})), maxBounds(bounds)
    {
        makeCenter();
    }

    void setupSprite(float spr_w, float spr_h, GridDimensions dims, DirType initial_dir)
    {
        sprite.sliceTextureBox(spr_w, spr_h, dims);
        sprite.setActiveDir(initial_dir);
    }

    void makeCenter()
    {
        position.x = maxBounds.x / 2.F;
        position.y = maxBounds.y / 2.F;
    }

    void setDirection(DirType dir)
    {
        sprite.setActiveDir(dir);
    }

    void tryUp(float Sdt)
    {
        y_vel = std::min((y_vel - (ACCEL * Sdt)), -MAX_SPEED);
        y_mov = true;
    }

    void tryDown(float Sdt)
    {
        y_vel = std::max((y_vel + (ACCEL * Sdt)), MAX_SPEED);
        y_mov = true;
    }

    void tryLeft(float Sdt)
    {
        x_vel = std::min((x_vel - (ACCEL * Sdt)), -MAX_SPEED);
        x_mov = true;
    }

    void tryRight(float Sdt)
    {
        x_vel = std::max((x_vel + (ACCEL * Sdt)), MAX_SPEED);
        x_mov = true;
    }

    void update(float delat_t)
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

    void render(SDL_Renderer *renderer)
    {
        sprite.renderActive(renderer, position);
    }

    [[nodiscard]] Texture &getTexture()
    {
        return sprite.getTexture();
    }

    [[nodiscard]] SDL_FPoint getPosition() const
    {
        return position;
    }
};