#pragma once

#include <SDL2/SDL.h>

class Pacman {

public:
    Pacman();

    void init(SDL_Renderer *renderer);
    void update();
    void render();

    void close();

private:
    SDL_Renderer *renderer;

    SDL_Texture *textureOriented;
    SDL_Texture *textureUnoriented;

    float x, y;
};
