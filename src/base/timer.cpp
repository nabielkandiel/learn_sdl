#include <SDL3/SDL.h>

#include "timer.hpp"

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