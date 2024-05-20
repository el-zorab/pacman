#include <SDL2/SDL.h>

#include "gameConstants.hpp"
#include "pacman.hpp"
#include "textureLoader.hpp"

static std::string PACMAN_TEXTURE_ORIENTED_PATH = "img/pacman_oriented.png";
static std::string PACMAN_TEXTURE_UNORIENTED_PATH = "img/pacman_unoriented.png";

Pacman::Pacman() {
    textureOriented = nullptr;
    textureUnoriented = nullptr;

    x = 0;
    y = 0;
}

void Pacman::init(SDL_Renderer *renderer) {
    this->renderer = renderer;

    textureOriented = loadTexture(renderer, PACMAN_TEXTURE_ORIENTED_PATH);
    textureUnoriented = loadTexture(renderer, PACMAN_TEXTURE_UNORIENTED_PATH);

    x = GameConstants::TILE_SIZE * 2 + GameConstants::TILE_SIZE / 2;
    y = GameConstants::TILE_SIZE * 3 + GameConstants::TILE_SIZE / 2;
}

void Pacman::update() {
    
}

void Pacman::render() {
    SDL_Rect destrect = {
        x,
        y,
        32,
        32
    };

    SDL_RenderCopyEx(renderer, textureOriented, NULL, &destrect, 2 * 90, NULL, SDL_FLIP_NONE);
}

void Pacman::close() {
    
}
