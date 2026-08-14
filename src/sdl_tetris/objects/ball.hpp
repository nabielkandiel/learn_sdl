#pragma once

#include <SDL3/SDL.h>

#include <algorithm>

class BallState
{
  public:
    BallState(float x_max, float y_max) : x_max(x_max), y_max(y_max) {}

    void makeCenter()
    {
        position.x = x_max / 2.F;
        position.y = y_max / 2.F;
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

    void update(float Sdt)
    {
        if (!x_mov) {
            if (x_vel > 0.0F) {
                x_vel -= (FRICTION * Sdt);
                x_vel = std::max(x_vel, 0.0F);
            } else if (x_vel < 0.0F) {
                x_vel += (FRICTION * Sdt);
                x_vel = std::min(x_vel, 0.0F);
            }
        }
        if (!y_mov) {
            if (y_vel > 0.0F) {
                y_vel -= (FRICTION * Sdt);
                y_vel = std::max(y_vel, 0.0F);
            } else if (y_vel < 0.0F) {
                y_vel += (FRICTION * Sdt);
                y_vel = std::min(y_vel, 0.0F);
            }
        }

        position.x += x_vel * Sdt;
        position.x = std::clamp(position.x, 0.0F, x_max);
        x_mov = false;
        position.y += y_vel * Sdt;
        position.y = std::clamp(position.y, 0.0F, y_max);
        y_mov = false;
    }

    [[nodiscard]] SDL_FPoint getPosition() const
    {
        return position;
    }

  private:
    SDL_FPoint position{.x = 0.0, .y = 0.0};
    float x_max;
    float y_max;

    float x_vel = 0;
    float y_vel = 0;

    bool x_mov{false};
    bool y_mov{false};

    static constexpr float ACCEL = 2500.0F;    // pixels/sec² — how fast it speeds up while held
    static constexpr float FRICTION = 4000.0F; // pixels/sec² — how fast it slows when released
    static constexpr float MAX_SPEED = 500.0F; // pixels/sec — cap so it doesn't accelerate forever
};