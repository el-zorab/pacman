#pragma once

#include <SDL2/SDL.h>
#include <string>

class GameController {

public:
    GameController();

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

    bool gameRunning;

    void renderBackground();
};
