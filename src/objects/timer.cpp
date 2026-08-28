#include "timer.hpp"

Timer::Timer() : timer(SDL_GetTicks()) {}

void Timer::handleInput(const SDL_Event &input)
{
    if (input.type == SDL_EVENT_KEY_DOWN && input.key.key == SDLK_RETURN) {
        timer = SDL_GetTicks();
    }
}

void Timer::update(float delat_t)
{
    time_text.str("");
    time_text << "MS since start" << SDL_GetTicks() - timer;
}
