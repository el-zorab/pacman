#pragma once

#include <memory>
#include <SDL2/SDL.h>

#include "timer.hpp"

enum class PacmanOrientation {
    LEFT,
    RIGHT,
    UP,
    DOWN
};

class Pacman {

public:
    Pacman(SDL_Renderer *renderer);
    void close();

    void setDesiredOrientation(PacmanOrientation orientation);
    void setOrientation(PacmanOrientation orientation);

    void update(float deltaTime);
    void render();

private:
    SDL_Renderer *renderer;
    SDL_Texture *textureOriented;
    SDL_Texture *textureUnoriented;

    PacmanOrientation orientation, desiredOrientation;
    int x, y;
    bool moving;

    std::unique_ptr<Timer> animationTimer;
    int animationIndex;
};

// class PacmanOrientation {

// public:
//     enum Orientation { LEFT, RIGHT, UP, DOWN };
//     PacmanOrientation(Orientation);

// };
