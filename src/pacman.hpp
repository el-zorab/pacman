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
    SDL_Renderer *renderer;
    SDL_Texture *textureOriented;
    SDL_Texture *textureUnoriented;

    const int TEXTURE_W = GameConst::TILE_SIZE;
    const int TEXTURE_H = GameConst::TILE_SIZE;

    Entity2D currPos;
    Entity2D currTile;
    Orientation orientation, desiredOrientation;

    const Uint32 ANIMATION_FRAME_DURATION_MS = 100;
    const int ANIMATION_FRAMES = 2;
    int animationIndex;
    std::unique_ptr<Timer> animationTimer;
};
