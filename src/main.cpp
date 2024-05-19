#define SDL_MAIN_HANDLED

#include <memory>
#include <SDL2/SDL.h>
#include <stdio.h>

#include "gameController.hpp"

int main(int argc, char* argv[]) {
    (void) argc; (void) argv;

    std::unique_ptr<GameController> gameController(new GameController());

    gameController->init("PACMAN", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

    while (gameController->isGameRunning()) {
        gameController->handleKeyboardEvents();
        gameController->update();
        gameController->render();
    }

    gameController->close();

    return 0;
}
