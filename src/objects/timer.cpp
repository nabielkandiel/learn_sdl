#include "timer.hpp"

void Timer::handleInput(const SDL_Event &input)
{
    if (input.type == SDL_EVENT_KEY_DOWN && input.key.key == SDLK_RETURN) {
        timer = SDL_GetTicks();
    }
}

void Timer::update(float delta_t)
{
    (void)delta_t;
    time_text.str("");
    time_text << "MS since start: " << SDL_GetTicks() - timer;
    text.setText(time_text.str());
}

void Timer::render(SDL_Renderer *renderer)
{
    text.renderText(position, renderer);
}