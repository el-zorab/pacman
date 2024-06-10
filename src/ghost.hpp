#pragma once

#include <SDL2/SDL.h>

#include "entity2d.hpp"
#include "gameConst.hpp"
#include "orientation.hpp"

class Ghost {

public:
    enum class State {
        CHASE, SCATTER, FRIGHTENED
    };

    Ghost();
    void init(Entity2D startTile, Orientation startOrientation, State startState);

    void update(int deltaTime);
    void render();

    Entity2D getCurrentTile();

private:
    SDL_Texture *ghostTexture;
    SDL_Texture *targetTileTexture;

    Entity2D currPos;
    Entity2D currTile;
    Orientation orientation;
    State state;

    const int VELOCITY = GameConst::VELOCITY_TILES_PER_SEC * 75 / 10;

    Entity2D getTargetTile();
    Orientation getNewOrientation();

    virtual Entity2D getChaseTargetTile() = 0;
    virtual Entity2D getScatterTargetTile() = 0;
    virtual SDL_Color getTextureColor() = 0;

};
