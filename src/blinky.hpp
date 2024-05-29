#pragma once

#include <SDL2/SDL.h>

#include "entity2d.hpp"
#include "orientation.hpp"

class Blinky {

public:
    Blinky(SDL_Renderer *renderer);

    void update();
    void render();
    
private:
    SDL_Renderer *renderer;

    SDL_Texture *blinkyTexture;
    SDL_Texture *targetTileTexture;

    Entity2D pos, tilePos, tilePosNext, targetTile;
    Orientation orientation;

};
