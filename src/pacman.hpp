#pragma once

#include <array>
#include <memory>
#include <SDL2/SDL.h>

#include "entity2d.hpp"
#include "gameConst.hpp"
#include "orientation.hpp"
#include "timer.hpp"

class Pacman {

public:
    Pacman();
    void reset();

    void update(int deltaTime);
    void render();

    Entity2D getCurrentTile();
    Entity2D getCurrentPos();
    SDL_Texture *getTextureOriented();
    Orientation getOrientation();
    void setDesiredOrientation(Orientation desiredOrientation);
    void setGhostsFrightened(bool ghostsFrightened);

private:
    SDL_Texture *textureOriented;
    SDL_Texture *textureUnoriented;

    Entity2D currPos;
    Entity2D currTile, nextTile;
    Orientation orientation, desiredOrientation;

    const int PACMAN_VEL        = GameConst::VELOCITY_TILES_PER_SEC * 80 / 10;
    const int PACMAN_FRIGHT_VEL = GameConst::VELOCITY_TILES_PER_SEC * 90 / 10;

    bool ghostsFrightened;

    const int ANIMATION_FRAME_DURATION_MS = 100;
    const int ANIMATION_FRAMES = 2;
    int animationIndex;
    std::unique_ptr<Timer> animationTimer;
};
