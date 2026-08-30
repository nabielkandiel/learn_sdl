#include "timer.hpp"

Timer::Timer(GameContext &context, Font &font, SDL_FPoint pos) : text(font), position(pos)
{
    auto &input_manager = context.getInputManager();
    input_manager.bindKeyDown(SDLK_RETURN, [this]() {
        if (started) {
            stop();
        } else {
            start();
        }
    });

    input_manager.bindKeyDown(SDLK_SPACE, [this]() {
        if (paused) {
            unpause();
        } else {
            pause();
        }
    });

    context.getEntityManager().registerEntity(*this);
};

void Timer::update(float /*delat_t*/)
{
    text.setText("MS since start: " + std::to_string(getTicksNS() / 1'000'000));
}

void Timer::render(SDL_Renderer *renderer)
{
    text.renderText(position, renderer);
}

void Timer::start()
{
    started = true;
    paused = false;

    timer_start = SDL_GetTicksNS();
    timer_paused = 0;
}

void Timer::stop()
{
    started = false;
    paused = false;

    timer_start = 0;
    timer_paused = 0;
}

void Timer::pause()
{
    if (started && !paused) {
        paused = true;
        timer_paused = SDL_GetTicksNS() - timer_start;
        timer_start = 0;
    }
}

void Timer::unpause()
{
    if (started && paused) {
        paused = false;
        timer_start = SDL_GetTicksNS() - timer_paused;
        timer_paused = 0;
    }
}

bool Timer::isStarted() const
{
    return started;
}

bool Timer::isPaused() const
{
    return paused;
}

uint64_t Timer::getTicksNS() const
{
    uint64_t time{0};

    if (started) {
        if (paused) {
            time = timer_paused;
        } else {
            time = SDL_GetTicksNS() - timer_start;
        }
    }
    return time;
}