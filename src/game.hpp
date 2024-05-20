#pragma once

#include <memory>
#include <SDL2/SDL.h>
#include <string>

#include "pacman.hpp"

class Game {

public:
    Game();

    void init(std::string title, int x, int y);
    void close();

    bool isGameRunning();
    void stopRunning();

    void handleKeyboardEvents();
    void update();
    void render();

private:
    SDL_Window *window;
    int windowWidth;
    int windowHeight;

    SDL_Renderer *renderer;

    std::unique_ptr<Pacman> pacman;

    bool gameRunning;

    void renderBackground();
};
