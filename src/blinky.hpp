#pragma once

#include <SDL2/SDL.h>

#include "entity2d.hpp"
#include "orientation.hpp"

class Blinky {

public:
    Blinky(SDL_Renderer *renderer);

    void update(Entity2D pacmanPosTile);
    void render();
    
private:
    SDL_Renderer *renderer;

    Entity2D pos, posTile, targetTile;
    Orientation orientation;

};
