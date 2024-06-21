#define SDL_MAIN_HANDLED

#include <memory>
#include <SDL2/SDL.h>
#include <stdio.h>

#include "game.hpp"

int main(int argc, char* argv[]) {
    (void) argc; (void) argv;

    srand(time(nullptr));

    Game &game = Game::getInstance();

    game.init("PACMAN", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

    while (!game.shouldQuit()) {
        game.handleEvents();
        game.update();
        game.render();
    }

    game.close();

    return 0;
}
