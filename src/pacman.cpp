#include <cmath>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>

#include "game.hpp"
#include "gameConstants.hpp"
#include "pacman.hpp"
#include "textureManager.hpp"
#include "tilingManager.hpp"

using GameConstants::TILE_SIZE;

const std::string PACMAN_TEXTURE_ORIENTED_PATH   = "pacman_oriented.png";
const std::string PACMAN_TEXTURE_UNORIENTED_PATH = "pacman_unoriented.png";

Pacman::Pacman(SDL_Renderer *renderer) {
    this->renderer = renderer;

    textureOriented = Game::getInstance().getTextureManager().loadTexture(renderer, PACMAN_TEXTURE_ORIENTED_PATH);
    textureUnoriented = Game::getInstance().getTextureManager().loadTexture(renderer, PACMAN_TEXTURE_UNORIENTED_PATH);

    pos.x = pos.y = TILE_SIZE;
    tilePos.x = 1;
    tilePos.y = 1;
    orientation = desiredOrientation = Orientation::RIGHT;

    animationIndex = 0;
    animationTimer = std::make_unique<Timer>();
    animationTimer->start();
}

void Pacman::update(float deltaTime) {
    Entity2D vec = orientationVec(orientation);

    if (Game::getInstance().getTilingManager().isTileFree(tilePos.x + vec.x, tilePos.y + vec.y)) {
        pos.x += vec.x * VELOCITY * deltaTime;
        pos.y += vec.y * VELOCITY * deltaTime;
    };
    
    if ((std::fmod(pos.x, TILE_SIZE) < 1.0 && std::fmod(pos.y, TILE_SIZE) < 1.0)
        || std::abs(pos.x - tilePos.x * TILE_SIZE) > TILE_SIZE || std::abs(pos.y - tilePos.y * TILE_SIZE) > TILE_SIZE) {
        tilePos.x = std::floor(pos.x / TILE_SIZE);
        tilePos.y = std::floor(pos.y / TILE_SIZE);

        pos.x = std::floor(pos.x / TILE_SIZE) * TILE_SIZE;
        pos.y = std::floor(pos.y / TILE_SIZE) * TILE_SIZE;

        if (orientation != desiredOrientation) {
            Entity2D desiredVec = orientationVec(desiredOrientation);
            Entity2D tilePosDesired = {
                tilePos.x + desiredVec.x,
                tilePos.y + desiredVec.y
            };

            if (Game::getInstance().getTilingManager().isTileFree(tilePosDesired.x, tilePosDesired.y)) { // == FREE
                orientation = desiredOrientation;
            }
        }
    }

    if (animationTimer->getTicks() > ANIMATION_FRAME_DURATION_MS) {
        animationIndex = (animationIndex + 1) % ANIMATION_FRAMES;
        animationTimer->start();
    }
}

void Pacman::setDesiredOrientation(Orientation desiredOrientation) {
    this->desiredOrientation = desiredOrientation;
};

void Pacman::render() {
    SDL_Color textColor = { 255, 255, 255, 255 };
    Game::getInstance().getFontRenderer().renderText(renderer, "X=" + std::to_string(pos.x), 0, 0,         textColor);
    Game::getInstance().getFontRenderer().renderText(renderer, "Y=" + std::to_string(pos.y), 0, TILE_SIZE, textColor);

    if (std::fmod(pos.x, TILE_SIZE) < 1.0 || std::fmod(pos.y, TILE_SIZE) < 1.0) {
        Game::getInstance().getFontRenderer().renderText(renderer, "ALIGNED", 0, 2 * TILE_SIZE, SDL_Color {0, 255, 0, 255});
    }

    SDL_Rect tilePosRect = { tilePos.x * TILE_SIZE, tilePos.y * TILE_SIZE, TILE_SIZE, TILE_SIZE };
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &tilePosRect);

    Entity2D vec = orientationVec(orientation);
    SDL_Rect tileNewPosRect = { (tilePos.x + vec.x) * TILE_SIZE, (tilePos.y + vec.y) * TILE_SIZE, TILE_SIZE, TILE_SIZE };
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderDrawRect(renderer, &tileNewPosRect);

    SDL_Rect textureRect = { (int) pos.x, (int) pos.y, TEXTURE_W, TEXTURE_H };
    SDL_Texture *toDraw = animationIndex ? textureUnoriented : textureOriented;

    SDL_RenderCopyEx(renderer, toDraw, NULL, &textureRect, orientationRotationDeg(orientation), NULL, SDL_FLIP_NONE);
}
