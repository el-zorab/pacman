#include <cmath>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>

#include "gameConstants.hpp"
#include "pacman.hpp"
#include "textureLoader.hpp"
#include "tile.hpp"
#include "tilingManager.hpp"

using GameConstants::TILE_SIZE;

static const std::string PACMAN_TEXTURE_ORIENTED_PATH   = "res/pacman_oriented.png";
static const std::string PACMAN_TEXTURE_UNORIENTED_PATH = "res/pacman_unoriented.png";

TTF_Font *jetbrainsMono = nullptr;

Pacman::Pacman(SDL_Renderer *renderer) {
    this->renderer = renderer;

    textureOriented = loadTexture(renderer, PACMAN_TEXTURE_ORIENTED_PATH, true);
    textureUnoriented = loadTexture(renderer, PACMAN_TEXTURE_UNORIENTED_PATH, true);

    orientation = desiredOrientation = Orientation::LEFT;

    x = xTile = xEndTile = 0;
    y = yTile = yEndTile = 0;

    y = 208;
    x = 16;
    moving = true;

    warping = false;

    animationTimer = std::make_unique<Timer>();
    animationIndex = 0;

    jetbrainsMono = TTF_OpenFont("res/jetbrains_mono_regular.ttf", 16);

    animationTimer->start();

    neighbourTiles[Orientation::LEFT][0]  = {-1,  0};
    neighbourTiles[Orientation::LEFT][1]  = {-1,  1};
    neighbourTiles[Orientation::RIGHT][0] = { 2,  0};
    neighbourTiles[Orientation::RIGHT][1] = { 2,  1};
    neighbourTiles[Orientation::UP][0]    = { 0, -1};
    neighbourTiles[Orientation::UP][1]    = { 1, -1};
    neighbourTiles[Orientation::DOWN][0]  = { 0,  2};
    neighbourTiles[Orientation::DOWN][1]  = { 1,  2};
}

void Pacman::close() {
    TTF_CloseFont(jetbrainsMono);
    SDL_DestroyTexture(textureOriented);
    SDL_DestroyTexture(textureUnoriented);
    textureOriented = nullptr;
    textureUnoriented = nullptr;
}

bool Pacman::areTilesFree(Orientation orientation) {
    for (int i = 0; i < NEIGHBOUR_TILES_COUNT; i++) {
        Tile neighbourTile = neighbourTiles[orientation][i];
        neighbourTile.x += xTile;
        neighbourTile.y += yTile;
        if (TilingManager::getInstance().getTilingAt(neighbourTile.x, neighbourTile.y)) {
            return false;
        }
    }
    return true;
}

void Pacman::setDesiredOrientation(Orientation desiredOrientation) {
    this->desiredOrientation = desiredOrientation;
}

void Pacman::update(float deltaTime) {
    (void) deltaTime;

    if (x % TILE_SIZE == 0 && y % TILE_SIZE == 0 && areTilesFree(desiredOrientation)) {
        orientation = desiredOrientation;
        moving = true;
    }

    moving = areTilesFree(orientation);

    if (warping) updateWarping();
    else if (moving) {
        updateMoving();
        warping = y == 208 && ((x == 0 && orientation == Orientation::LEFT) || (x == 400 && orientation == Orientation::RIGHT));
    }

    if (animationTimer->getTicks() > 100) {
        animationIndex = (animationIndex + 1) & 1;
        animationTimer->start();
    }

    SDL_Delay(250);
}

void Pacman::updateMoving() {
    switch (orientation) {
        case Orientation::LEFT:
            x -= PACMAN_SPEED;
            break;
        case Orientation::RIGHT:
            x += PACMAN_SPEED;
            break;
        case Orientation::UP:
            y -= PACMAN_SPEED;
            break;
        case Orientation::DOWN:
            y += PACMAN_SPEED;
            break;
    }

    x = std::min(std::max(0, x), GameConstants::WINDOW_WIDTH - PACMAN_TEXTURE_W);
    y = std::min(std::max(0, y), GameConstants::WINDOW_HEIGHT - PACMAN_TEXTURE_H);
            
    if (x % TILE_SIZE == 0 && y % TILE_SIZE == 0) {
        xTile = x / TILE_SIZE;
        yTile = y / TILE_SIZE;
    }
}

void Pacman::updateWarping() {
    if (orientation == Orientation::LEFT) {
        x -= PACMAN_SPEED;

        if (x <= -PACMAN_TEXTURE_W) {
            x = GameConstants::WINDOW_WIDTH + 3 * PACMAN_TEXTURE_W;
        } else if (x == GameConstants::WINDOW_WIDTH - PACMAN_TEXTURE_W) {
            warping = false;
        }
    } else if (orientation == Orientation::RIGHT) {
        x += PACMAN_SPEED;

        if (x >= GameConstants::WINDOW_WIDTH) {
            x = -PACMAN_TEXTURE_W;
        } else if (x <= 0) {
            warping = false;
        }
    }
}

void Pacman::render() {
    std::string textX = "TILE X: " + std::to_string(x / TILE_SIZE) + " X: " + std::to_string(x);
    std::string textY = "TILE Y: " + std::to_string(y / TILE_SIZE) + " Y: " + std::to_string(y);
    std::string warpingText = "WARPING";

    SDL_Color onTileColor = { 255, 255, 0, 255 };
    SDL_Color notOnTileColor = { 0, 255, 0, 255 };
    SDL_Color warpingColor = { 255, 0, 0, 255 };

    SDL_Surface *textXSurface = TTF_RenderText_Solid(jetbrainsMono, textX.c_str(), x % TILE_SIZE == 0 ? notOnTileColor : onTileColor);
    SDL_Texture *textXTexture = SDL_CreateTextureFromSurface(renderer, textXSurface);

    SDL_Surface *textYSurface = TTF_RenderText_Solid(jetbrainsMono, textY.c_str(), y % TILE_SIZE == 0 ? notOnTileColor : onTileColor);
    SDL_Texture *textYTexture = SDL_CreateTextureFromSurface(renderer, textYSurface);

    SDL_Surface *warpingSurface = TTF_RenderText_Solid(jetbrainsMono, warpingText.c_str(), warpingColor);
    SDL_Texture *warpingTexture = SDL_CreateTextureFromSurface(renderer, warpingSurface);

    SDL_Rect textXRect = { 0, 0,         textXSurface->w, textXSurface->h };
    SDL_Rect textYRect = { 0, TILE_SIZE, textYSurface->w, textYSurface->h };
    SDL_Rect warpingRect = { 0, 2 * TILE_SIZE, warpingSurface->w, warpingSurface->h };

    SDL_RenderCopy(renderer, textXTexture, NULL, &textXRect);
    SDL_RenderCopy(renderer, textYTexture, NULL, &textYRect);
    if (warping) SDL_RenderCopy(renderer, warpingTexture, NULL, &warpingRect);

    SDL_FreeSurface(textXSurface);
    SDL_FreeSurface(textYSurface);
    SDL_FreeSurface(warpingSurface);
    SDL_DestroyTexture(textXTexture);
    SDL_DestroyTexture(textYTexture);
    SDL_DestroyTexture(warpingTexture);
   
    SDL_Rect textureRect = { x, y, PACMAN_TEXTURE_W, PACMAN_TEXTURE_H };
    SDL_Texture *toDraw = animationIndex ? textureUnoriented : textureOriented;

    SDL_RenderCopyEx(renderer, toDraw, NULL, &textureRect, textureRotationFromOrientation(orientation), NULL, SDL_FLIP_NONE);
}

int Pacman::textureRotationFromOrientation(Orientation orientation) {
    switch (orientation) {
        case Orientation::LEFT:  return 180;
        case Orientation::RIGHT: return 0;
        case Orientation::UP:    return 270;
        case Orientation::DOWN:  return 90;
    }
    return 0;
}
