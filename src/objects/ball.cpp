#include "ball.hpp"

void Ball::handleInput(const SDL_Event &event)
{
    if (event.type == SDL_EVENT_KEY_DOWN) {
        switch (event.key.key) {
        case SDLK_UP:
            setDirection(ballDir::UP);
            break;
        case SDLK_DOWN:
            setDirection(ballDir::DOWN);
            break;
        case SDLK_LEFT:
            setDirection(ballDir::LEFT);
            break;
        case SDLK_RIGHT:
            setDirection(ballDir::RIGHT);
            break;
        default:
            break;
        }
    }
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