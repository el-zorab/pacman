#include <string>

#include "game.hpp"
#include "ghost.hpp"

using GameConst::TILE_SIZE, GameConst::UNITS_PER_PIXEL, GameConst::UNITS_PER_TILE;

Ghost::Ghost() {}

void Ghost::init(Entity2D startTile, Orientation startOrientation, State startState) {
    ghostTexture     = Game::getInstance().getTextureManager().loadTexture("ghost.png");
    targetTileTexture = Game::getInstance().getTextureManager().loadTexture("target_tile.png");

    SDL_Color textureColor = getTextureColor();
    SDL_SetTextureColorMod(ghostTexture,      textureColor.r, textureColor.g, textureColor.b);
    SDL_SetTextureColorMod(targetTileTexture, textureColor.r, textureColor.g, textureColor.b);

    currTile = startTile;
    currPos = currTile * UNITS_PER_TILE;
    orientation = startOrientation;
    state = startState;
}

Entity2D Ghost::getTargetTile() {
    switch (state) {
        case State::CHASE:
            return getChaseTargetTile();
        case State::SCATTER:
            return getScatterTargetTile();
        case State::FRIGHTENED:
            return { 0, 0 };
    }
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

        Entity2D distVector = nextTile - getTargetTile();
        int distance = distVector.x * distVector.x + distVector.y * distVector.y;

        if (distance < minDistance) {
            minDistance = distance;
            newOrientation = orientationCandidate;
        }
    }

    return newOrientation;
}

void Ghost::update(int deltaTime) {
    if (currPos.x < GameConst::WARPING_MIN_X) {
        currPos.x = GameConst::WARPING_MAX_X;
        currTile.x = currPos.x / UNITS_PER_TILE;
    } else if (currPos.x > GameConst::WARPING_MAX_X) {
        currPos.x = GameConst::WARPING_MIN_X;
        currTile.x = currPos.x / UNITS_PER_TILE;
    } else {
        Entity2D orientationVector = orientationToVector(orientation);
        int deltaUnits = VELOCITY * deltaTime;
        currPos = currPos + orientationVector * deltaUnits;
    }

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
        currPos.x / UNITS_PER_PIXEL - TILE_SIZE / 2,
        currPos.y / UNITS_PER_PIXEL - TILE_SIZE / 2,
        GameConst::ENTITY_TEXTURE_SIZE,
        GameConst::ENTITY_TEXTURE_SIZE
    };

    SDL_RenderCopyEx(renderer, ghostTexture, nullptr, &blinkyRect, orientationToDeg(orientation), nullptr, SDL_FLIP_NONE);

    SDL_Rect targetTileRect = { getTargetTile().x * TILE_SIZE, getTargetTile().y * TILE_SIZE, TILE_SIZE, TILE_SIZE };
    SDL_RenderCopy(renderer, targetTileTexture, nullptr, &targetTileRect);

    // SDL_Rect currTileRect = { currTile.x * TILE_SIZE, currTile.y * TILE_SIZE, TILE_SIZE, TILE_SIZE };
    // SDL_Color rectColor = getTextureColor();
    // SDL_SetRenderDrawColor(renderer, rectColor.r, rectColor.g, rectColor.b, rectColor.a);
    // SDL_RenderDrawRect(renderer, &currTileRect);
}

Entity2D Ghost::getCurrentTile() {
    return currTile;
}
