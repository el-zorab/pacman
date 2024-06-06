#pragma once

#include <SDL2/SDL.h>

#include "entity2d.hpp"
#include "gameConst.hpp"
#include "orientation.hpp"

class Ghost {

public:
    Ghost();
    void init(Entity2D startTile, Orientation startOrientation);

    void update(int deltaTime);
    void render();

    Entity2D getCurrentTile();

    void setStartTile(Entity2D startTile);
    void setStartOrientation(Orientation startOrientation);

private:
    SDL_Texture *blinkyTexture;
    SDL_Texture *targetTileTexture;

    Entity2D currPos;
    Entity2D currTile;
    Orientation orientation;

    const int VELOCITY = GameConst::VELOCITY_TILES_PER_SEC * 75 / 10;

    Orientation getNewOrientation();

    virtual Entity2D getTargetTile() = 0;
    virtual SDL_Color getTextureColor() = 0;

};
