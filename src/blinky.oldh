#pragma once

#include <SDL2/SDL.h>

#include "entity2d.hpp"
#include "orientation.hpp"

class Blinky {

public:
    Blinky();

    void update();
    void render();
    
private:
    SDL_Renderer *renderer;
    SDL_Texture *blinkyTexture;
    SDL_Texture *targetTileTexture;

    Entity2D pos, currTile, nextTile, targetTile;
    Orientation orientation;

    Orientation findNewOrientation();
};
