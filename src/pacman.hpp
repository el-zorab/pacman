#pragma once

#include <array>
#include <memory>
#include <SDL2/SDL.h>

#include "gameConstants.hpp"
#include "orientation.hpp"
#include "timer.hpp"

class Pacman {

public:
    Pacman(SDL_Renderer *renderer);

    void setDesiredOrientation(Orientation desiredOrientation);

    void update(float deltaTime);
    void render();

private:
    SDL_Renderer *renderer;
    SDL_Texture *textureOriented;
    SDL_Texture *textureUnoriented;

    const int TEXTURE_W = GameConstants::TILE_SIZE;
    const int TEXTURE_H = GameConstants::TILE_SIZE;
    const int SPEED = 4;

    Entity2D pos, tilePos;
    Orientation orientation, desiredOrientation;

    const Uint32 ANIMATION_FRAME_DURATION_MS = 100;
    const int ANIMATION_FRAMES = 2;
    int animationIndex;
    std::unique_ptr<Timer> animationTimer;
};
