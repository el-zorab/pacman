#pragma once

#include <memory>
#include <SDL2/SDL.h>

#include "tile.hpp"
#include "timer.hpp"

class Pacman {

public:
    Pacman(SDL_Renderer *renderer);
    void close();

    enum Orientation {
        LEFT = 0,
        RIGHT = 1,
        UP = 2,
        DOWN = 3
    };

    void setDesiredOrientation(Orientation orientation);

    void update(float deltaTime);
    void render();

private:
    SDL_Renderer *renderer;
    SDL_Texture *textureOriented;
    SDL_Texture *textureUnoriented;

    Orientation orientation, desiredOrientation;
    int x, y, xStartTile, yStartTile, xEndTile, yEndTile;
    bool moving;

    std::unique_ptr<Timer> animationTimer;
    int animationIndex;

    int rotationFromOrientation(Orientation orientation);
};
