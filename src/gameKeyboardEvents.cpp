#include "gameKeyboardEvents.hpp"

void GameKeyboardEvents::handle() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            
        }
    }
}
