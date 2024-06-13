#include "timer.hpp"

Timer::Timer() {
    ticksAtStart = 0;
    ticksWhilePaused = 0;
    paused = false;
}

void Timer::start() {
    ticksAtStart = SDL_GetTicks();
    ticksWhilePaused = 0;
    paused = false;
}

void Timer::pause() {
    if (!paused) {
        paused = true;

        ticksWhilePaused = SDL_GetTicks() - ticksAtStart;
    }
}

void Timer::unpause() {
    if (paused) {
        paused = false;

        ticksAtStart = SDL_GetTicks() - ticksWhilePaused;
    }
}

int Timer::getMiliseconds() {
    if (paused) return ticksWhilePaused;
    else return SDL_GetTicks() - ticksAtStart;
}
