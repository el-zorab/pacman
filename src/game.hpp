#pragma once

#include <memory>
#include <SDL2/SDL.h>
#include <string>

#include "pacman.hpp"
#include "timer.hpp"

class Game {

public:
    Game(std::string title, int x, int y);
    void close();

    bool isGameRunning();
    void stopRunning();

    void handleEvents();
    void update();
    void render();

private:
    SDL_Window *window;
    int windowWidth;
    int windowHeight;
    SDL_Renderer *renderer;
    bool gameRunning;
    std::unique_ptr<Timer> frameTimer;

    std::unique_ptr<Pacman> pacman;

    void renderBackground();
};
