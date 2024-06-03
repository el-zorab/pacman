#pragma once

#include <SDL2/SDL.h>

#include "entity2d.hpp"
#include "gameConst.hpp"
#include "orientation.hpp"

class Ghost {

public:
    Ghost();

    void update(int deltaTime);
    void render();
    
private:
    SDL_Texture *blinkyTexture;

    Entity2D currPos;
    Entity2D currTile;
    Orientation orientation;

    Orientation getNewOrientation();
    
    virtual SDL_Color getTextureColor() = 0;
    virtual std::string getTexturePath() = 0;
    virtual int getVelocity() = 0;
};
