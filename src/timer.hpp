#pragma once

#include <SDL2/SDL.h>

class Timer {

public:
    Timer();

    void start();
    void pause();
    void unpause();

    int getMiliseconds();

private:
    int ticksAtStart;
    int ticksWhilePaused;
    bool paused;
};
