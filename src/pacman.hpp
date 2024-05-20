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
    ~Pacman();
    void close();

    void setOrientation(PacmanOrientation orientation);

    void update(float deltaTime);
    void render();

private:
    SDL_Renderer *renderer;
    SDL_Texture *textureOriented;
    SDL_Texture *textureUnoriented;

    PacmanOrientation orientation;
    float x, y;

    std::unique_ptr<Timer> animationTimer;
    int animationIndex;
};

// class PacmanOrientation {

// public:
//     enum Orientation { LEFT, RIGHT, UP, DOWN };
//     PacmanOrientation(Orientation);

// };
