#include <string>

#include "blinky.hpp"
#include "game.hpp"
#include "gameConst.hpp"

using GameConst::TILE_SIZE;

const std::string BLINKY_TEXTURE_PATH      = "blinky.png";
const std::string TARGET_TILE_TEXTURE_PATH = "x.png";

const SDL_Color BLINKY_COLOR = { 255, 0, 0, 255 };

Blinky::Blinky() {
    renderer = Game::getInstance().getRenderer();

    blinkyTexture     = Game::getInstance().getTextureManager().loadTexture(renderer, BLINKY_TEXTURE_PATH);
    targetTileTexture = Game::getInstance().getTextureManager().loadTexture(renderer, TARGET_TILE_TEXTURE_PATH);
    SDL_SetTextureColorMod(targetTileTexture, BLINKY_COLOR.r, BLINKY_COLOR.g, BLINKY_COLOR.b);

    currTile.x = 13;
    currTile.y = 11; 
    pos.x = currTile.x * TILE_SIZE;
    pos.y = currTile.y * TILE_SIZE;
    orientation = Orientation::RIGHT;
}

Orientation Blinky::findNewOrientation() {
    int minDistance = 10000000;

    Orientation newOrientation;
    for (int i = 0; i < ORIENTATIONS; i++) {
        Orientation orientationCandidate = static_cast<Orientation>(i);
            
        if (orientationAreOpposites(orientation, orientationCandidate)) {
            continue;
        }

        Entity2D orientationCandidateVector = orientationToVector(orientationCandidate);
        nextTile = {
            currTile.x + orientationCandidateVector.x,
            currTile.y + orientationCandidateVector.y
        };

        if (!Game::getInstance().getTilingManager().isTileFree(nextTile.x, nextTile.y)) {
            continue;
        }

        int distanceX = (nextTile.x - targetTile.x); // nextTile - getGhostTargetTile
        int distanceY = (nextTile.y - targetTile.y);
        int distance = distanceX * distanceX + distanceY * distanceY;

        if (distance < minDistance) {
            minDistance = distance;
            newOrientation = orientationCandidate;
        }
    }

    return newOrientation;
}

void Blinky::update() {
    targetTile = Game::getInstance().getPacman().getTargetTile();

    if (currTile.y == 14) {
        if (pos.x <= -TILE_SIZE) {
            pos.x = GameConst::WINDOW_WIDTH;
        } else if (pos.x >= GameConst::WINDOW_WIDTH) {
            pos.x = -TILE_SIZE;
        }
    }

    if (pos.x % TILE_SIZE == 0 && pos.y % TILE_SIZE == 0) {
        currTile.x = pos.x / TILE_SIZE;
        currTile.y = pos.y / TILE_SIZE;
        orientation = findNewOrientation();
    }
    
    Entity2D orientationVector = orientationToVector(orientation);
    pos.x += orientationVector.x;
    pos.y += orientationVector.y;
}

void Blinky::render() {
    SDL_Rect blinkyRect = { pos.x + 1, pos.y + 1, TILE_SIZE - 2, TILE_SIZE - 2 };
    SDL_SetRenderDrawColor(renderer, BLINKY_COLOR.r, BLINKY_COLOR.g, BLINKY_COLOR.b, BLINKY_COLOR.a);
    SDL_RenderFillRect(renderer, &blinkyRect);

    SDL_Rect blinkyTargetRect = { targetTile.x * TILE_SIZE, targetTile.y * TILE_SIZE, TILE_SIZE, TILE_SIZE };
    SDL_RenderCopy(renderer, targetTileTexture, nullptr, &blinkyTargetRect);

    Entity2D vec = orientationToVector(Game::getInstance().getPacman().getOrientation());
    SDL_Rect pinkyTargetRect = { (targetTile.x + vec.x * 4) * TILE_SIZE, (targetTile.y + vec.y * 4) * TILE_SIZE, TILE_SIZE, TILE_SIZE };
    SDL_SetTextureColorMod(targetTileTexture, 255, 0, 255);
    SDL_RenderCopy(renderer, targetTileTexture, nullptr, &pinkyTargetRect);

    Entity2D inkyTarget = {
        2 * (targetTile.x + 2 * vec.x) - currTile.x,
        2 * (targetTile.y + 2 * vec.y) - currTile.y
    };
    SDL_Rect inkyTargetRect = { inkyTarget.x * TILE_SIZE, inkyTarget.y * TILE_SIZE, TILE_SIZE, TILE_SIZE };
    SDL_SetTextureColorMod(targetTileTexture, 0, 255, 255);
    SDL_RenderCopy(renderer, targetTileTexture, nullptr, &inkyTargetRect);

    SDL_SetTextureColorMod(targetTileTexture, BLINKY_COLOR.r, BLINKY_COLOR.g, BLINKY_COLOR.b);
}
