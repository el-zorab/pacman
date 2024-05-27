#define SDL_MAIN_HANDLED

#include <memory>
#include <SDL2/SDL.h>
#include <stdio.h>

#include "game.hpp"

int main(int argc, char* argv[]) {
    (void) argc; (void) argv;

    std::unique_ptr<Game> game = std::make_unique<Game>("PACMAN", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

    while (game->isGameRunning()) {
        game->handleEvents();
        game->update();
        game->render();
        // SDL_Delay(50);
    }

    game->close();

    return 0;
}
