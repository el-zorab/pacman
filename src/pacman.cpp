#include <cmath>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>

#include "game.hpp"
#include "pacman.hpp"
#include "textureManager.hpp"
#include "tilingManager.hpp"

using GameConst::TILE_SIZE, GameConst::UNITS_PER_PIXEL, GameConst::UNITS_PER_TILE;

const std::string PACMAN_TEXTURE_ORIENTED_PATH   = "pacman_oriented.png";
const std::string PACMAN_TEXTURE_UNORIENTED_PATH = "pacman_unoriented.png";

Pacman::Pacman() {
    textureOriented = Game::getInstance().getTextureManager().loadTexture(PACMAN_TEXTURE_ORIENTED_PATH);
    textureUnoriented = Game::getInstance().getTextureManager().loadTexture(PACMAN_TEXTURE_UNORIENTED_PATH);

    currTile.x = 13;
    currTile.y = 20;
    orientation = desiredOrientation = Orientation::UP;

    currPos = currTile * UNITS_PER_TILE;
    currPos.x += UNITS_PER_TILE / 2;

    animationIndex = 0;
    animationTimer = std::make_unique<Timer>();
    animationTimer->start();
}

void Pacman::update(int deltaTime) {
    Entity2D orientationVector = orientationToVector(orientation);
    Entity2D desiredOrientationVector = orientationToVector(desiredOrientation);
    
    nextTile = currTile + orientationVector;
    Entity2D desiredNextTile = currTile + desiredOrientationVector;

    // "walk" if the next tile is free
    if (Game::getInstance().getTilingManager().getTileState(nextTile.x, nextTile.y) == TileState::FREE) {
        if (currPos.x < GameConst::WARPING_MIN_X) {
            currPos.x = GameConst::WARPING_MAX_X;
            currTile.x = currPos.x / UNITS_PER_TILE;
        } else if (currPos.x > GameConst::WARPING_MAX_X) {
            currPos.x = GameConst::WARPING_MIN_X;
            currTile.x = currPos.x / UNITS_PER_TILE;
        } else {
            int deltaUnits = PACMAN_VEL * deltaTime;
            currPos = currPos + orientationVector * deltaUnits;
        }
    // stop if the next tile is solid
    } else {
        // and try to change pacman's orientation
        if (Game::getInstance().getTilingManager().getTileState(desiredNextTile.x, desiredNextTile.y) == TileState::FREE) {
            orientation = desiredOrientation;
        }
    }
    
    // position delta = (current position - start position)
    Entity2D deltaPos = currPos - currTile * UNITS_PER_TILE;
    // if pacman walked out of his tile
    if (std::abs(deltaPos.x) >= UNITS_PER_TILE || std::abs(deltaPos.y) >= UNITS_PER_TILE) {
        switch (orientation) {
            case Orientation::LEFT:
                currTile.x--;
                break;
            case Orientation::RIGHT:
                currTile.x++;
                break;
            case Orientation::UP:
                currTile.y--;
                break;
            case Orientation::DOWN:
                currTile.y++;
                break;
        }

        // align the current position since pacman doesnt walk every pixel
        currPos = currTile * UNITS_PER_TILE;
        
        // try to change the orientation
        desiredNextTile = currTile + desiredOrientationVector;
        if (Game::getInstance().getTilingManager().getTileState(desiredNextTile.x, desiredNextTile.y) == TileState::FREE) {
            orientation = desiredOrientation;
        }
    }

    if (animationTimer->getTicks() > ANIMATION_FRAME_DURATION_MS) {
        animationIndex = (animationIndex + 1) % ANIMATION_FRAMES;
        animationTimer->start();
    }
}

void Pacman::render() {
    SDL_Renderer *renderer = Game::getInstance().getRenderer();
    
    SDL_Rect pacmanRect = {
        currPos.x / UNITS_PER_PIXEL - TILE_SIZE / 2,
        currPos.y / UNITS_PER_PIXEL - TILE_SIZE / 2,
        GameConst::ENTITY_TEXTURE_SIZE,
        GameConst::ENTITY_TEXTURE_SIZE
    };
    SDL_Texture *texture = animationIndex ? textureUnoriented : textureOriented;
    SDL_RenderCopyEx(renderer, texture, nullptr, &pacmanRect, orientationToDeg(orientation), nullptr, SDL_FLIP_NONE);
    
    // SDL_Rect currTileRect = { currTile.x * TILE_SIZE + 1, currTile.y * TILE_SIZE + 1, TILE_SIZE - 2, TILE_SIZE - 2 };
    // SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    // SDL_RenderDrawRect(renderer, &currTileRect);
}

Entity2D Pacman::getCurrentTile() {
    return currTile;
}

Orientation Pacman::getOrientation() {
    return orientation;
}

void Pacman::setDesiredOrientation(Orientation desiredOrientation) {
    this->desiredOrientation = desiredOrientation;
};
