#pragma once

#include <SDL2/SDL.h>

#include "entity2d.hpp"
#include "gameConst.hpp"
#include "orientation.hpp"

class Ghost {

public:
    enum class Mode {
        CHASE, SCATTER, FRIGHTENED, IN_HOUSE, EXIT_HOUSE
    };

    Ghost();
    void init();

    void update(int deltaTime);
    void render();

    Entity2D getCurrentTile();
    bool isInGhostHouse();
    void setMode(Mode mode);
    
    void exitHouse();

protected:
    Entity2D currPos;
    Orientation orientation;
    Mode mode;

private:
    SDL_Texture *ghostTexture;
    SDL_Texture *targetTileTexture;

    Entity2D currTile;
    Mode modeAfterExit;

    const int GHOST_VEL = GameConst::VELOCITY_TILES_PER_SEC * 75 / 10;

    virtual void initChild() = 0;

    Orientation findNewOrientation();

    Entity2D getTargetTile();

    virtual Entity2D getChaseTargetTile() = 0;
    virtual Entity2D getScatterTargetTile() = 0;
    virtual SDL_Color getTextureColor() = 0;

};
