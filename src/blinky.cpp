#include <string>

#include "blinky.hpp"
#include "game.hpp"
#include "gameConst.hpp"

using GameConst::TILE_SIZE;

const std::string BLINKY_TEXTURE_PATH      = "blinky.png";
const std::string TARGET_TILE_TEXTURE_PATH = "x.png";

const SDL_Color BLINKY_COLOR = { 255, 0, 0, 255 };

Blinky::Blinky(SDL_Renderer *renderer) {
    this->renderer = renderer;

    blinkyTexture     = Game::getInstance().getTextureManager().loadTexture(renderer, BLINKY_TEXTURE_PATH);
    targetTileTexture = Game::getInstance().getTextureManager().loadTexture(renderer, TARGET_TILE_TEXTURE_PATH);
    SDL_SetTextureColorMod(targetTileTexture, BLINKY_COLOR.r, BLINKY_COLOR.g, BLINKY_COLOR.b);

    tilePos.x = 13;
    tilePos.y = 11; 
    pos.x = tilePos.x * TILE_SIZE;
    pos.y = tilePos.y * TILE_SIZE;
    orientation = Orientation::RIGHT;
}

void Blinky::update() {
    targetTile = Game::getInstance().getPacman().getPacmanTargetTile();

    if (pos.x % TILE_SIZE == 0 && pos.y % TILE_SIZE == 0) {
        tilePos.x = pos.x / TILE_SIZE;
        tilePos.y = pos.y / TILE_SIZE;

        int minDistanceSq = 1000000;

        Orientation newOrientation;
        for (int i = 0; i < ORIENTATIONS; i++) {
            Orientation orientationCandidate = static_cast<Orientation>(i);
            
            if (orientationAreOpposites(orientation, orientationCandidate)) {
                continue;
            }

            Entity2D orientationCandidateVector = orientationToVector(orientationCandidate);
            tilePosNext = {
                tilePos.x + orientationCandidateVector.x,
                tilePos.y + orientationCandidateVector.y
            };

            if (!Game::getInstance().getTilingManager().isTileFree(tilePosNext.x, tilePosNext.y)) {
                continue;
            }

            int distanceX = (tilePosNext.x - targetTile.x);
            int distanceY = (tilePosNext.y - targetTile.y);
            int distanceSq = distanceX * distanceX + distanceY * distanceY;

            SDL_Log("%s distance: %s\n", orientationName(orientationCandidate).c_str(), std::to_string(distanceSq).c_str());

            if (distanceSq < minDistanceSq) {
                minDistanceSq = distanceSq;
                // SDL_Log("%d\n", (int) orientationAreOpposites(orientation, orientationCandidate));
                newOrientation = orientationCandidate;
            }
        }

        SDL_Log("END\n");

        orientation = newOrientation;

        // SDL_Delay(1000);
    }

    Entity2D orientationVector = orientationToVector(orientation);
    pos.x += orientationVector.x;
    pos.y += orientationVector.y;
}

void Blinky::render() {
    SDL_Rect blinkyRect = { pos.x, pos.y, TILE_SIZE, TILE_SIZE };
    SDL_RenderCopyEx(renderer, blinkyTexture, nullptr, &blinkyRect, orientationToDeg(orientation), nullptr, SDL_FLIP_NONE);

    SDL_Rect targetRect = { targetTile.x * TILE_SIZE, targetTile.y * TILE_SIZE, TILE_SIZE, TILE_SIZE };
    SDL_RenderCopy(renderer, targetTileTexture, nullptr, &targetRect);

    // SDL_Rect tilePosRect = { tilePosNext.x * TILE_SIZE, tilePosNext.y * TILE_SIZE, TILE_SIZE, TILE_SIZE };
    // SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    // SDL_RenderDrawRect(renderer, &tilePosRect);
}
