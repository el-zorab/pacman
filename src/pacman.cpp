#include <cmath>
#include <SDL2/SDL.h>

#include "gameConstants.hpp"
#include "pacman.hpp"
#include "textureLoader.hpp"

static const int PACMAN_TEXTURE_W = 24;
static const int PACMAN_TEXTURE_H = 24;

static const std::string PACMAN_TEXTURE_ORIENTED_PATH = "res/pacman_oriented.png";
static const std::string PACMAN_TEXTURE_UNORIENTED_PATH = "res/pacman_unoriented.png";

Pacman::Pacman(SDL_Renderer *renderer) {
    this->renderer = renderer;

    textureOriented = loadTexture(renderer, PACMAN_TEXTURE_ORIENTED_PATH, true);
    textureUnoriented = loadTexture(renderer, PACMAN_TEXTURE_UNORIENTED_PATH, true);

    orientation = PacmanOrientation::RIGHT;

    x = GameConstants::TILE_SIZE * 5 + GameConstants::TILE_SIZE / 2;
    y = GameConstants::TILE_SIZE * 2 + GameConstants::TILE_SIZE / 2;

    animationTimer = std::make_unique<Timer>();
    animationIndex = 0;

    animationTimer->start();
}

Pacman::~Pacman() {
    close();
}

void Pacman::close() {
    SDL_DestroyTexture(textureOriented);
    SDL_DestroyTexture(textureUnoriented);
    textureOriented = nullptr;
    textureUnoriented = nullptr;
}

void Pacman::setOrientation(PacmanOrientation orientation) {
    this->orientation = orientation;
}

void Pacman::update(float deltaTime) {
    if (animationTimer->getTicks() > 100) {
        animationIndex = (animationIndex + 1) & 1;
        animationTimer->start();
    }

    int speed = 8 * GameConstants::TILE_SIZE;

    switch (orientation) {
        case PacmanOrientation::LEFT:
            x -= speed * deltaTime;
            break;
        case PacmanOrientation::RIGHT:
            x += speed * deltaTime;
            break;
        case PacmanOrientation::UP:
            y -= speed * deltaTime;
            break;
        case PacmanOrientation::DOWN:
            y += speed * deltaTime;
            break;
    }

    x = std::max(0.f, x);
    y = std::max(0.f, y);
    x = std::min(x, (float) GameConstants::WINDOW_WIDTH - PACMAN_TEXTURE_W);
    y = std::min(y, (float) GameConstants::WINDOW_HEIGHT - PACMAN_TEXTURE_H);
}

void Pacman::render() {
    SDL_Rect destrect = {
        (int) x, (int) y, PACMAN_TEXTURE_W, PACMAN_TEXTURE_H
    };

    SDL_Texture *toDraw = animationIndex ? textureUnoriented : textureOriented;

    int rotation;

    switch (orientation) {
        case PacmanOrientation::LEFT:
            rotation = 180;
            break;
        case PacmanOrientation::RIGHT:
            rotation = 0;
            break;
        case PacmanOrientation::UP:
            rotation = 270;
            break;
        case PacmanOrientation::DOWN:
            rotation = 90;
            break;
    }


    SDL_RenderCopyEx(renderer, toDraw, NULL, &destrect, rotation, NULL, SDL_FLIP_NONE);
}
