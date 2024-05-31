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

Pacman::Pacman() {
    renderer = Game::getInstance().getRenderer();

    textureOriented = Game::getInstance().getTextureManager().loadTexture(renderer, PACMAN_TEXTURE_ORIENTED_PATH);
    textureUnoriented = Game::getInstance().getTextureManager().loadTexture(renderer, PACMAN_TEXTURE_UNORIENTED_PATH);

    tilePos.x = 3;
    tilePos.y = 14;
    pos.x = tilePos.x * TILE_SIZE;
    pos.y = tilePos.y * TILE_SIZE;
    orientation = desiredOrientation = Orientation::RIGHT;

    animationIndex = 0;
    animationTimer = std::make_unique<Timer>();
    animationTimer->start();
}

void Pacman::update() {
    Entity2D vec = orientationToVector(orientation);
    Entity2D vecDesired = orientationToVector(desiredOrientation);

    tilePosNext = {
        tilePos.x + vec.x,
        tilePos.y + vec.y
    };

    if (tilePos.y == 14) {
        if (pos.x <= -TILE_SIZE) {
            pos.x = GameConst::WINDOW_WIDTH;
        } else if (pos.x >= GameConst::WINDOW_WIDTH) {
            pos.x = -TILE_SIZE;
        }
    }

    if (Game::getInstance().getTilingManager().isTileFree(tilePosNext.x, tilePosNext.y)) {
        pos.x += vec.x;
        pos.y += vec.y;
    };

    if (pos.x % TILE_SIZE == 0 && pos.y % TILE_SIZE == 0) {
        tilePos.x = pos.x / TILE_SIZE;
        tilePos.y = pos.y / TILE_SIZE;

        if (orientation != desiredOrientation) {
            Entity2D tilePosDesired = {
                tilePos.x + vecDesired.x,
                tilePos.y + vecDesired.y
            };

            if (Game::getInstance().getTilingManager().isTileFree(tilePosDesired.x, tilePosDesired.y)) { // == FREE/SOLID
                orientation = desiredOrientation;
            }
        }
    } else if (orientationAreOpposites(orientation, desiredOrientation)) {
        orientation = desiredOrientation;
        tilePos = tilePosNext;
    }

    if (animationTimer->getTicks() > ANIMATION_FRAME_DURATION_MS) {
        animationIndex = (animationIndex + 1) % ANIMATION_FRAMES;
        animationTimer->start();
    }
}

void Pacman::render() {
    SDL_Color textColor = { 255, 255, 255, 255 };
    Game::getInstance().getFontRenderer().renderText(renderer, "X=" + std::to_string(pos.x), 0, 0,         textColor);
    Game::getInstance().getFontRenderer().renderText(renderer, "Y=" + std::to_string(pos.y), 0, TILE_SIZE, textColor);

    SDL_Rect pacmanRect = { pos.x, pos.y, TEXTURE_W, TEXTURE_H };
    SDL_Texture *texture = animationIndex ? textureUnoriented : textureOriented;

    SDL_RenderCopyEx(renderer, texture, nullptr, &pacmanRect, orientationToDeg(orientation), nullptr, SDL_FLIP_NONE);
}

Entity2D Pacman::getTargetTile() {
    Entity2D targetTile = tilePosNext;
    return targetTile;
}

Orientation Pacman::getOrientation() {
    return orientation;
}

void Pacman::setDesiredOrientation(Orientation desiredOrientation) {
    this->desiredOrientation = desiredOrientation;
};

