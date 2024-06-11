#include <string>

#include "game.hpp"
#include "ghost.hpp"

using GameConst::TILE_SIZE, GameConst::UNITS_PER_PIXEL, GameConst::UNITS_PER_TILE;

const Entity2D EXITING_HOUSE_TILE = { 13, 16 };
const Entity2D OUT_OF_HOUSE_TILE  = { 13, 14 };

const int HOUSE_MIN_TILE_Y = 16;
const int HOUSE_MAX_TILE_Y = 18;

Ghost::Ghost() {}

void Ghost::init() {
    ghostTexture     = Game::getInstance().getTextureManager().loadTexture("ghost.png");
    targetTileTexture = Game::getInstance().getTextureManager().loadTexture("target_tile.png");

    SDL_Color textureColor = getTextureColor();
    SDL_SetTextureColorMod(ghostTexture,      textureColor.r, textureColor.g, textureColor.b);
    SDL_SetTextureColorMod(targetTileTexture, textureColor.r, textureColor.g, textureColor.b);

    initChild();
    
    currTile = currPos / UNITS_PER_TILE;
}

Entity2D Ghost::getTargetTile() {
    switch (mode) {
        case Mode::CHASE:
            return getChaseTargetTile();
        case Mode::SCATTER:
            return getScatterTargetTile();
        case Mode::EXIT_HOUSE:
            return EXITING_HOUSE_TILE;
        case Mode::FRIGHTENED:
        case Mode::IN_HOUSE:
        default:
            return { 0, 0 };
    }
}

Orientation Ghost::findNewOrientation() {
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
        int deltaUnits = GHOST_VEL * deltaTime;
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

        if (mode == Mode::IN_HOUSE) {
            if (currTile.y == HOUSE_MIN_TILE_Y) orientation = Orientation::DOWN;
            else if (currTile.y == HOUSE_MAX_TILE_Y) orientation = Orientation::UP;
        } else if (mode == Mode::EXIT_HOUSE) {
            if (currTile == EXITING_HOUSE_TILE) orientation = Orientation::UP;
            else if (currTile == OUT_OF_HOUSE_TILE) {
                mode = Game::getInstance().getCurrentMode();
                orientation = findNewOrientation();
            } else {
                orientation = findNewOrientation();
            }
        } else {
            currPos = currTile * UNITS_PER_TILE;
            orientation = findNewOrientation();
        }
    }
}

void Ghost::render() {
    SDL_Renderer *renderer = Game::getInstance().getRenderer();

    SDL_Rect ghostRect = {
        currPos.x / UNITS_PER_PIXEL - TILE_SIZE / 2,
        currPos.y / UNITS_PER_PIXEL - TILE_SIZE / 2,
        GameConst::ENTITY_TEXTURE_SIZE,
        GameConst::ENTITY_TEXTURE_SIZE
    };

    SDL_RenderCopyEx(renderer, ghostTexture, nullptr, &ghostRect, orientationToDeg(orientation), nullptr, SDL_FLIP_NONE);

    // SDL_Rect targetTileRect = { getTargetTile().x * TILE_SIZE, getTargetTile().y * TILE_SIZE, TILE_SIZE, TILE_SIZE };
    // SDL_RenderCopy(renderer, targetTileTexture, nullptr, &targetTileRect);

    // SDL_Rect currTileRect = { currTile.x * TILE_SIZE, currTile.y * TILE_SIZE, TILE_SIZE, TILE_SIZE };
    // SDL_Color rectColor = getTextureColor();
    // SDL_SetRenderDrawColor(renderer, rectColor.r, rectColor.g, rectColor.b, rectColor.a);
    // SDL_RenderDrawRect(renderer, &currTileRect);

    // SDL_RenderDrawRect(renderer, &ghostRect);
}

Entity2D Ghost::getCurrentTile() {
    return currTile;
}

bool Ghost::isInGhostHouse() {
    return mode == Mode::IN_HOUSE || mode == Mode::EXIT_HOUSE;
}

void Ghost::setMode(Mode mode) {
    this->mode = mode;
}
