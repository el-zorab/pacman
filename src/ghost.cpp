#include <string>

#include "game.hpp"
#include "ghost.hpp"

using GameConst::UNITS_PER_TILE;

Ghost::Ghost() {}

void Ghost::init(Entity2D startTile, Orientation startOrientation) {
    blinkyTexture = Game::getInstance().getTextureManager().loadTexture(getTexturePath());
    SDL_Color textureColor = getTextureColor();
    SDL_SetTextureColorMod(blinkyTexture, textureColor.r, textureColor.g, textureColor.b);

    currTile = startTile;
    currPos = currTile * UNITS_PER_TILE;
    orientation = startOrientation;
}

Orientation Ghost::getNewOrientation() {
    int minDistance = 10000000;

    Orientation newOrientation;
    for (int i = 0; i < ORIENTATIONS; i++) {
        Orientation orientationCandidate = static_cast<Orientation>(i);
            
        if (orientationAreOpposites(orientation, orientationCandidate)) {
            continue;
        }

        Entity2D orientationCandidateVector = orientationToVector(orientationCandidate);
        Entity2D nextTile = {
            currTile.x + orientationCandidateVector.x,
            currTile.y + orientationCandidateVector.y
        };

        if (Game::getInstance().getTilingManager().getTileState(nextTile.x, nextTile.y) == TileState::SOLID) {
            continue;
        }

        Entity2D targetTile = getTargetTile();

        int distanceX = (nextTile.x - targetTile.x);
        int distanceY = (nextTile.y - targetTile.y);
        int distance = distanceX * distanceX + distanceY * distanceY;

        if (distance < minDistance) {
            minDistance = distance;
            newOrientation = orientationCandidate;
        }
    }

    return newOrientation;
}

void Ghost::update(int deltaTime) {
    Entity2D orientationVector = orientationToVector(orientation);

    int deltaUnits = getVelocity() * deltaTime;
    currPos = currPos + orientationVector * deltaUnits;

    Entity2D deltaPos = currPos - currTile * UNITS_PER_TILE;
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

        currPos = currTile * UNITS_PER_TILE;

        orientation = getNewOrientation();
    }
}

void Ghost::render() {
    SDL_Renderer *renderer = Game::getInstance().getRenderer();

    SDL_Rect blinkyRect = {
        currPos.x / GameConst::UNITS_PER_PIXEL,
        currPos.y / GameConst::UNITS_PER_PIXEL,
        GameConst::TILE_SIZE,
        GameConst::TILE_SIZE
    };
    SDL_RenderCopyEx(renderer, blinkyTexture, nullptr, &blinkyRect, orientationToDeg(orientation), nullptr, SDL_FLIP_NONE);

    SDL_Rect targetTileRect = {
        getTargetTile().x * GameConst::TILE_SIZE,
        getTargetTile().y * GameConst::TILE_SIZE,
        GameConst::TILE_SIZE,
        GameConst::TILE_SIZE
    };
    SDL_Color textureColor = getTextureColor();
    SDL_SetRenderDrawColor(renderer, textureColor.r, textureColor.g, textureColor.b, textureColor.a);
    SDL_RenderDrawRect(renderer, &targetTileRect);
}

Entity2D Ghost::getCurrentTile() {
    return currTile;
}
