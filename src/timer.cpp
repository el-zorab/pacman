#include "timer.hpp"

Timer::Timer() {
    ticks = 0;
}

void Timer::start() {
    ticks = SDL_GetTicks();
}

Uint32 Timer::getTicks() {
    return SDL_GetTicks() - ticks;
}
