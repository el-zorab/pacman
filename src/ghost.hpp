#pragma once

#include <SDL2/SDL.h>

#include "entity2d.hpp"
#include "gameConst.hpp"
#include "orientation.hpp"

class Ghost {

public:
    static constexpr int GHOST_COUNT = 4;

    enum Name : int const {
        BLINKY = 0,
        PINKY = 1,
        INKY = 2,
        CLYDE = 3
    };

    enum class Mode {
        CHASE, SCATTER, FRIGHTENED, RESPAWNED
    };

    Ghost();
    void init();

    void update(int deltaTime);
    void render();

    Entity2D getCurrentTile();
    bool isRespawned();

    void setEaten();
    void setExitHouse();
    void resetGhostColor();
    void setGhostColor(SDL_Color color);
    void setMode(Mode mode);

protected:
    enum class State {
        NORMAL_OPERATION,
        WANDERING,
        EXITING_HOUSE,
        EATEN,
        ENTERING_HOUSE,
        RESPAWNED
    };

    Entity2D currPos;
    Orientation orientation;
    Mode mode;
    State state;

    Entity2D scatterTargetTile;

    SDL_Color ghostColor;

private:
    static constexpr Entity2D INSIDE_HOUSE_TILE  = { 13, 16 };
    static constexpr Entity2D OUTSIDE_HOUSE_TILE = { 13, 14 };

    static constexpr Entity2D RESPAWN_TILE = { 13, 18 };

    static constexpr int HOUSE_MIN_TILE_Y = 16;
    static constexpr int HOUSE_MAX_TILE_Y = 18;

    static constexpr int GHOST_VEL            = GameConst::VELOCITY_TILES_PER_SEC * 75 / 10;
    static constexpr int GHOST_VEL_FRIGHTENED = GameConst::VELOCITY_TILES_PER_SEC * 50 / 10;

    SDL_Texture *ghostTexture;
    SDL_Texture *targetTileTexture;

    Entity2D currTile;

    Orientation computeOrientationToTile(Entity2D targetTile);

    virtual void initChild() = 0;
    virtual Entity2D getChaseTargetTile() = 0;

};
