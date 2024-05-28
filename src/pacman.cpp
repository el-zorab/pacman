#include <cmath>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>

#include "game.hpp"
#include "pacman.hpp"
#include "textureManager.hpp"
#include "tilingManager.hpp"

const std::string PACMAN_TEXTURE_ORIENTED_PATH   = "pacman_oriented.png";
const std::string PACMAN_TEXTURE_UNORIENTED_PATH = "pacman_unoriented.png";

Pacman::Pacman(SDL_Renderer *renderer) {
    this->renderer = renderer;

    textureOriented = Game::getInstance().getTextureManager().loadTexture(renderer, PACMAN_TEXTURE_ORIENTED_PATH);
    textureUnoriented = Game::getInstance().getTextureManager().loadTexture(renderer, PACMAN_TEXTURE_UNORIENTED_PATH);

    pos.x = pos.y = Game::TILE_SIZE;
    tilePos.x = 1;
    tilePos.y = 1;
    orientation = desiredOrientation = Orientation::RIGHT;

    animationIndex = 0;
    animationTimer = std::make_unique<Timer>();
    animationTimer->start();
}

void Pacman::update() {
    Entity2D vec = orientationVec(orientation);
    Entity2D vecDesired = orientationVec(desiredOrientation);

    Entity2D tilePosNext = {
        tilePos.x + vec.x,
        tilePos.y + vec.y
    };

    if (Game::getInstance().getTilingManager().isTileFree(tilePosNext.x, tilePosNext.y)) {
        pos.x += vec.x;
        pos.y += vec.y;
    };

    if (pos.x % Game::TILE_SIZE == 0 && pos.y % Game::TILE_SIZE == 0) {
        tilePos.x = std::floor(pos.x / Game::TILE_SIZE);
        tilePos.y = std::floor(pos.y / Game::TILE_SIZE);

        if ((tilePos.x <= 0 || tilePos.x >= Game::TILE_COLS) && tilePos.y == 14) { // warping
            const int xWarpingMin = -TEXTURE_W;
            const int xWarpingMax = Game::WINDOW_WIDTH;

            if (pos.x == xWarpingMin) {
                pos.x = xWarpingMax;
            } else if (pos.x == xWarpingMax) {
                pos.x = xWarpingMin;
            }
        } else if (orientation != desiredOrientation) {
            Entity2D tilePosDesired = {
                tilePos.x + vecDesired.x,
                tilePos.y + vecDesired.y
            };

            if (Game::getInstance().getTilingManager().isTileFree(tilePosDesired.x, tilePosDesired.y)) { // == FREE
                orientation = desiredOrientation;
            }
        }
    } else if (vec.x + vecDesired.x == 0 && vec.y + vecDesired.y == 0) {
        orientation = desiredOrientation;
        tilePos = tilePosNext;
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
    Game::getInstance().getFontRenderer().renderText(renderer, "X=" + std::to_string(pos.x), 0, 0,               textColor);
    Game::getInstance().getFontRenderer().renderText(renderer, "Y=" + std::to_string(pos.y), 0, Game::TILE_SIZE, textColor);

    SDL_Rect tilePosRect = { tilePos.x * Game::TILE_SIZE, tilePos.y * Game::TILE_SIZE, Game::TILE_SIZE, Game::TILE_SIZE };
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &tilePosRect);

    Entity2D vec = orientationVec(orientation);
    SDL_Rect tileNewPosRect = { (tilePos.x + vec.x) * Game::TILE_SIZE, (tilePos.y + vec.y) * Game::TILE_SIZE, Game::TILE_SIZE, Game::TILE_SIZE };
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderDrawRect(renderer, &tileNewPosRect);

    SDL_Rect textureRect = { (int) pos.x, (int) pos.y, TEXTURE_W, TEXTURE_H };
    SDL_Texture *toDraw = animationIndex ? textureUnoriented : textureOriented;

    SDL_RenderCopyEx(renderer, toDraw, NULL, &textureRect, orientationRotationDeg(orientation), NULL, SDL_FLIP_NONE);
}
