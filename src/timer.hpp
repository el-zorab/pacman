#pragma once

#include <SDL2/SDL.h>

class Timer {

public:
    Timer();

    void start();
    Uint32 getTicks();

private:
    Uint32 ticks;
    
};
