#include <cmath>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>

#include "game.hpp"
#include "pacman.hpp"
#include "textureManager.hpp"
#include "tilingManager.hpp"

using GameConst::TILE_SIZE;

const std::string PACMAN_TEXTURE_ORIENTED_PATH   = "pacman_oriented.png";
const std::string PACMAN_TEXTURE_UNORIENTED_PATH = "pacman_unoriented.png";

const int UNITS_PER_PIXEL = 100000;
const int UNITS_PER_TILE = UNITS_PER_PIXEL * TILE_SIZE;

const int VELOCITY_ONE_TILE_PER_SEC = UNITS_PER_TILE / 1000; // velocity to travel units of one tile in 1000ms
const int VELOCITY = 1 * VELOCITY_ONE_TILE_PER_SEC; // 5 tiles / sec

Pacman::Pacman() {
    renderer = Game::getInstance().getRenderer();

    textureOriented = Game::getInstance().getTextureManager().loadTexture(renderer, PACMAN_TEXTURE_ORIENTED_PATH);
    textureUnoriented = Game::getInstance().getTextureManager().loadTexture(renderer, PACMAN_TEXTURE_UNORIENTED_PATH);

    currTile.x = 10;
    currTile.y = 14;
    orientation = desiredOrientation = Orientation::LEFT;

    currPos = currTile * UNITS_PER_TILE;

    animationIndex = 0;
    animationTimer = std::make_unique<Timer>();
    animationTimer->start();
}

int cnt = 0;

void Pacman::update(int deltaTime) {
    Entity2D orientationVector = orientationToVector(orientation);

    int deltaUnits = VELOCITY * deltaTime;
    currPos = currPos + orientationVector * deltaUnits;

    Entity2D deltaPos = currPos - currTile * UNITS_PER_TILE;
    // SDL_Log("DeltaPos.x: %d\n", std::abs(deltaPos.x));?

    if (std::abs(deltaPos.x) >= UNITS_PER_TILE || std::abs(deltaPos.y) >= UNITS_PER_TILE) {
        currTile = currPos / UNITS_PER_TILE;

        SDL_Log("ON TILE: %d, X: %d, DPX:%d\n", currTile.x, currPos.x, deltaUnits);
    }

    if (animationTimer->getTicks() > ANIMATION_FRAME_DURATION_MS) {
        animationIndex = (animationIndex + 1) % ANIMATION_FRAMES;
        animationTimer->start();
    }
}

void Pacman::render() {
    SDL_Color textColor = { 255, 255, 255, 255 };
    Game::getInstance().getFontRenderer().renderText(renderer, "X=" + std::to_string(currPos.x), 0, 0,         textColor);
    Game::getInstance().getFontRenderer().renderText(renderer, "Y=" + std::to_string(currPos.y), 0, TILE_SIZE, textColor);

    SDL_Rect pacmanRect = { currPos.x / UNITS_PER_PIXEL, currPos.y / UNITS_PER_PIXEL, TEXTURE_W, TEXTURE_H };
    SDL_Texture *texture = animationIndex ? textureUnoriented : textureOriented;

    SDL_RenderCopyEx(renderer, texture, nullptr, &pacmanRect, orientationToDeg(orientation), nullptr, SDL_FLIP_NONE);
    
    SDL_Rect rect = { currTile.x * TILE_SIZE, currTile.y * TILE_SIZE, TILE_SIZE, TILE_SIZE };
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &rect);
}

Entity2D Pacman::getTargetTile() {
    return {0, 0};
}

Orientation Pacman::getOrientation() {
    return orientation;
}

void Pacman::setDesiredOrientation(Orientation desiredOrientation) {
    this->desiredOrientation = desiredOrientation;
};
