#include <string>

#include "game.hpp"
#include "ghost.hpp"

using GameConst::UNITS_PER_TILE;

const std::string BLINKY_TEXTURE_PATH = "arrow.png";

const SDL_Color BLINKY_COLOR = { 255, 0, 255, 255 };

Blinky::Blinky() {
    blinkyTexture = Game::getInstance().getTextureManager().loadTexture(BLINKY_TEXTURE_PATH);
    SDL_SetTextureColorMod(blinkyTexture, BLINKY_COLOR.r, BLINKY_COLOR.g, BLINKY_COLOR.b);

    currTile = { 6, 5 };
    currPos = currTile * UNITS_PER_TILE;
    orientation = Orientation::DOWN;
}

Orientation Blinky::getNewOrientation() {
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

        Entity2D targetTile = Game::getInstance().getPacman().getTargetTile();

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

void Blinky::update(int deltaTime) {
    Entity2D orientationVector = orientationToVector(orientation);

    int deltaUnits = VELOCITY * deltaTime;
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

void Blinky::render() {
    SDL_Renderer *renderer = Game::getInstance().getRenderer();

    SDL_Color textColor = { 255, 255, 255, 255 };
    Game::getInstance().getFontRenderer().renderText(renderer, "X=" + std::to_string(currPos.x), 0, 0,         textColor);
    Game::getInstance().getFontRenderer().renderText(renderer, "Y=" + std::to_string(currPos.y), 0, GameConst::TILE_SIZE, textColor);

    SDL_Rect blinkyRect = { currPos.x / GameConst::UNITS_PER_PIXEL, currPos.y / GameConst::UNITS_PER_PIXEL, GameConst::TILE_SIZE, GameConst::TILE_SIZE };
    SDL_RenderCopyEx(renderer, blinkyTexture, nullptr, &blinkyRect, orientationToDeg(orientation), nullptr, SDL_FLIP_NONE);
}
