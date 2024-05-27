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
    tilePos.x = pos.x / TILE_SIZE;
    tilePos.y = pos.y / TILE_SIZE;
    orientation = Orientation::RIGHT;

    animationIndex = 0;
    animationTimer = std::make_unique<Timer>();
    animationTimer->start();
}

void Pacman::update(float deltaTime) {
    (void) deltaTime;

    Entity2D vec = orientationVec(orientation);
    Entity2D tileNewPos = { tilePos.x + vec.x, tilePos.y + vec.y };

    if (Game::getInstance().getTilingManager().isTileFree(tileNewPos.x, tileNewPos.y)) {
        SDL_Log("yes");
        pos.x += vec.x * SPEED;
        pos.y += vec.y * SPEED;
    }

    Entity2D posMax = {
        GameConstants::WINDOW_WIDTH - TEXTURE_W,
        GameConstants::WINDOW_HEIGHT - TEXTURE_H
    };

    if (pos.x <= 0) pos.x = 0;
    if (pos.y <= 0) pos.y = 0;
    if (pos.x >= posMax.x) pos.x = posMax.x;
    if (pos.y >= posMax.y) pos.y = posMax.y;

    if (pos.x % TILE_SIZE == 0 && pos.y % TILE_SIZE == 0) {
        tilePos.x = pos.x / TILE_SIZE;
        tilePos.y = pos.y / TILE_SIZE;

        if (orientation != desiredOrientation) {
            Entity2D desiredOrientationVec = orientationVec(desiredOrientation);
            Entity2D tileNewPos = {
                tilePos.x + desiredOrientationVec.x,
                tilePos.y + desiredOrientationVec.y
            };

            if (Game::getInstance().getTilingManager().isTileFree(tileNewPos.x, tileNewPos.y)) { // == FREE
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

    if (pos.x % TILE_SIZE == 0 && pos.y % TILE_SIZE == 0) {
        Game::getInstance().getFontRenderer().renderText(renderer, "ON TILE", 0, 2 * TILE_SIZE, SDL_Color {0, 255, 0, 255});
    }

    SDL_Rect tilePosRect = { tilePos.x * TILE_SIZE, tilePos.y * TILE_SIZE, TILE_SIZE, TILE_SIZE };
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &tilePosRect);

    Entity2D vec = orientationVec(orientation);
    SDL_Rect tileNewPosRect = { (tilePos.x + vec.x) * TILE_SIZE, (tilePos.y + vec.y) * TILE_SIZE, TILE_SIZE, TILE_SIZE };
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderDrawRect(renderer, &tileNewPosRect);

    SDL_Rect textureRect = { pos.x, pos.y, TEXTURE_W, TEXTURE_H };
    SDL_Texture *toDraw = animationIndex ? textureUnoriented : textureOriented;

    SDL_RenderCopyEx(renderer, toDraw, NULL, &textureRect, orientationRotationDeg(orientation), NULL, SDL_FLIP_NONE);
}
