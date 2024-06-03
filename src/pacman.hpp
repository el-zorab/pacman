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

    void update(int deltaTime);
    void render();

    Entity2D getTargetTile();
    Orientation getOrientation();
    void setDesiredOrientation(Orientation desiredOrientation);

private:
    SDL_Texture *textureOriented;
    SDL_Texture *textureUnoriented;

    const int TEXTURE_W = GameConst::TILE_SIZE;
    const int TEXTURE_H = GameConst::TILE_SIZE;

    Entity2D currPos;
    Entity2D currTile, nextTile;
    Orientation orientation, desiredOrientation;

    const int VELOCITY = 80 * GameConst::VELOCITY_ONE_TILE_PER_SEC / 10;

    const Uint32 ANIMATION_FRAME_DURATION_MS = 100;
    const int ANIMATION_FRAMES = 2;
    int animationIndex;
    std::unique_ptr<Timer> animationTimer;
};
