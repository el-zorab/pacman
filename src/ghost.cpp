#include <string>

#include "game.hpp"
#include "ghost.hpp"

using GameConst::TILE_SIZE, GameConst::UNITS_PER_PIXEL, GameConst::UNITS_PER_TILE;

Ghost::Ghost() {}

void Ghost::init() {
    ghostTexture      = Game::getInstance().getTextureManager().loadTexture("ghost.png");
    ghostEatenTexture = Game::getInstance().getTextureManager().loadTexture("ghost_eaten.png");
    targetTileTexture = Game::getInstance().getTextureManager().loadTexture("target_tile.png");

    initChild();

    SDL_SetTextureColorMod(ghostTexture,      ghostColor.r, ghostColor.g, ghostColor.b);
    SDL_SetTextureColorMod(ghostEatenTexture, GHOST_FRIGHTENED_COLOR.r, GHOST_FRIGHTENED_COLOR.g, GHOST_FRIGHTENED_COLOR.b);
    SDL_SetTextureColorMod(targetTileTexture, ghostColor.r, ghostColor.g, ghostColor.b);
    
    currTile = currPos / UNITS_PER_TILE;
    mode = Ghost::Mode::SCATTER;

    reverseOrientation = false;
}

Orientation Ghost::computeOrientationToTile(Entity2D targetTile) {
    int minDistance = 10000000;

    Orientation newOrientation;
    for (int i = 0; i < ORIENTATIONS; i++) {
        Orientation orientationCandidate = static_cast<Orientation>(i);

        if (orientationAreOpposites(orientation, orientationCandidate)) {
            continue;
        }

        Entity2D orientationCandidateVector = orientationToVector(orientationCandidate);
        Entity2D nextTile = currTile + orientationCandidateVector;

        if (Game::getInstance().getTilingManager().getTileState(nextTile.x, nextTile.y) == TileState::SOLID) {
            continue;
        }

        Entity2D distVector = nextTile - targetTile;
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
        int velocity = mode == Mode::FRIGHTENED && state != State::EATEN ? GHOST_VEL_FRIGHTENED : GHOST_VEL;
        int deltaUnits = velocity * deltaTime;
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

        if (state == State::RESPAWNED) {
            state = State::WANDERING;
            setGhostColor(ghostColor);
        }

        if (state == State::WANDERING) {
            if (currTile.y == HOUSE_MIN_TILE_Y) {
                orientation = Orientation::DOWN;
            } else if (currTile.y == HOUSE_MAX_TILE_Y) {
                orientation = Orientation::UP;
            }
        }
        
        if (state == State::EXITING_HOUSE) {
            if (currTile == INSIDE_HOUSE_TILE) {
                orientation = Orientation::UP;
            } else if (currTile == OUTSIDE_HOUSE_TILE) {
                state = State::NORMAL_OPERATION;
            } else if (currTile.y > OUTSIDE_HOUSE_TILE.y) {
                orientation = computeOrientationToTile(INSIDE_HOUSE_TILE);
            }
        }

        if (state == State::EATEN) {
            if (currTile == OUTSIDE_HOUSE_TILE) {
                orientation = Orientation::DOWN;
                state = State::ENTERING_HOUSE;
            } else {
                orientation = computeOrientationToTile(OUTSIDE_HOUSE_TILE);
            }
        }

        if (state == State::ENTERING_HOUSE) {
            if (currTile.y == HOUSE_MIN_TILE_Y) {
                state = State::RESPAWNED;
            }
        }

        if (state == State::NORMAL_OPERATION) {
            currPos = currTile * UNITS_PER_TILE;

            if (reverseOrientation) {
                Orientation orientation;
                for (int i = 0; i < ORIENTATIONS; i++) {
                    orientation = static_cast<Orientation>(i);
                    if (orientationAreOpposites(this->orientation, orientation)) {
                        this->orientation = orientation;
                        break;
                    }
                }
                reverseOrientation = false;
            }

            else if (mode == Mode::CHASE) {
                orientation = computeOrientationToTile(getChaseTargetTile());
            
            } else if (mode == Mode::SCATTER) {
                orientation = computeOrientationToTile(scatterTargetTile);
            
            } else if (mode == Mode::FRIGHTENED) {
                int possibleOrientations[4];
                int possibleOrientationsIndex = 0;
                for (int i = 0; i < ORIENTATIONS; i++) {
                    Orientation orientation = static_cast<Orientation>(i);
                    Entity2D orientationVector = orientationToVector(orientation);
                    Entity2D nextTile = currTile + orientationVector;

                    if (orientationAreOpposites(this->orientation, orientation)) {
                        continue;
                    }

                    if (Game::getInstance().getTilingManager().getTileState(nextTile.x, nextTile.y) == TileState::SOLID) {
                        continue;
                    }

                    possibleOrientations[possibleOrientationsIndex++] = i;
                }

                int randomIndex = rand() % possibleOrientationsIndex;
                int randomOrientation = possibleOrientations[randomIndex];
                orientation = static_cast<Orientation>(randomOrientation);
            
            }
        }
        
        if (scatterTargetTile.x == 25 && scatterTargetTile.y == 0) {
            std::string str;
            if (state == State::NORMAL_OPERATION) {
                str = "NORMAL_OPERATION";
            } else if (state == State::WANDERING) {
                str = "WANDERING";
            } else if (state == State::EXITING_HOUSE) {
                str = "EXITING_HOUSE";
            } else if (state == State::EATEN) {
                str = "EATEN";
            } else if (state == State::ENTERING_HOUSE) {
                str = "ENTERING_HOUSE";
            }
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
    SDL_Texture *textureToRender = state == State::EATEN ? ghostEatenTexture : ghostTexture;
    SDL_RenderCopy(renderer, textureToRender, nullptr, &ghostRect);

    // SDL_Rect targetTileRect = { getChaseTargetTile().x * TILE_SIZE, getChaseTargetTile().y * TILE_SIZE, TILE_SIZE, TILE_SIZE };
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

bool Ghost::isEaten() {
    return state == State::EATEN;
}

bool Ghost::isFrightened() {
    return mode == Mode::FRIGHTENED;
}

bool Ghost::isRespawned() {
    return state == State::RESPAWNED;
}

void Ghost::setEaten() {
    if (state == State::NORMAL_OPERATION) {
        state = State::EATEN;
    }
}

void Ghost::setExitHouse() {
    if (state == State::WANDERING) {
        state = State::EXITING_HOUSE;
    }
}

void Ghost::setGhostFlashColor() {
    setGhostColor(GHOST_FLASH_COLOR);
}

void Ghost::setGhostFrightenedColor() {
    setGhostColor(GHOST_FRIGHTENED_COLOR);
}

void Ghost::setGhostColor(SDL_Color color) {
    SDL_SetTextureColorMod(ghostTexture, color.r, color.g, color.b);
}

void Ghost::setMode(Mode mode) {
    reverseOrientation = this->mode != mode && state == State::NORMAL_OPERATION;
    
    this->mode = mode;

    if (state != State::EATEN) {
        if (mode == Mode::FRIGHTENED) {
            setGhostColor(GHOST_FRIGHTENED_COLOR);
        } else {
            setGhostColor(ghostColor);
        
        }
    }
}
