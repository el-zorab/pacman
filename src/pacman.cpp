#include <cmath>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "gameConstants.hpp"
#include "pacman.hpp"
#include "textureLoader.hpp"
#include "tile.hpp"
#include "tilingManager.hpp"

TTF_Font *jetbrainsMono = nullptr;

static const int PACMAN_W = 32;
static const int PACMAN_H = 32;

static const std::string PACMAN_TEXTURE_ORIENTED_PATH = "res/pacman_oriented.png";
static const std::string PACMAN_TEXTURE_UNORIENTED_PATH = "res/pacman_unoriented.png";

Tile tilesToCheck[4][2];

Pacman::Pacman(SDL_Renderer *renderer) {
    this->renderer = renderer;

    textureOriented = loadTexture(renderer, PACMAN_TEXTURE_ORIENTED_PATH, true);
    textureUnoriented = loadTexture(renderer, PACMAN_TEXTURE_UNORIENTED_PATH, true);

    orientation = desiredOrientation = Orientation::RIGHT;

    x = xStartTile = xEndTile = 0;
    y = yStartTile = yEndTile = 0;
    moving = true;

    animationTimer = std::make_unique<Timer>();
    animationIndex = 0;

    jetbrainsMono = TTF_OpenFont("res/JetBrainsMono-Regular.ttf", 16);

    animationTimer->start();

    tilesToCheck[Orientation::LEFT][0] = {-1, 0};
    tilesToCheck[Orientation::LEFT][1] = {-1, 1};
    tilesToCheck[Orientation::RIGHT][0] = {2, 0};
    tilesToCheck[Orientation::RIGHT][1] = {2, 1};
    tilesToCheck[Orientation::UP][0] = {0, -1};
    tilesToCheck[Orientation::UP][1] = {1, -1};
    tilesToCheck[Orientation::DOWN][0] = {0, 2};
    tilesToCheck[Orientation::DOWN][1] = {1, 2};
}

void Pacman::close() {
    TTF_CloseFont(jetbrainsMono);
    SDL_DestroyTexture(textureOriented);
    SDL_DestroyTexture(textureUnoriented);
    textureOriented = nullptr;
    textureUnoriented = nullptr;
}

bool changeOrientation = true;

void Pacman::setDesiredOrientation(Orientation desiredOrientation) {
    this->desiredOrientation = desiredOrientation;
    changeOrientation = true;
}

void Pacman::update(float deltaTime) {
    (void) deltaTime;

    if (x % 16 == 0 && y % 16 == 0) {
        if (orientation != desiredOrientation) {
            changeOrientation = true;

            for (int i = 0; i < 2; i++) {
                int xt, yt;
                xt = xStartTile + tilesToCheck[desiredOrientation][i].x;
                yt = yStartTile + tilesToCheck[desiredOrientation][i].y;
                if (TilingManager::getInstance().getTilingAt(xt, yt)) {
                    changeOrientation = false;
                    break;
                }
            }

            if (changeOrientation) {
                orientation = desiredOrientation;
                // SDL_Delay(10000);
            }
        }
    }

    if (moving) {
        int speed = 1;
        switch (orientation) {
            case Orientation::LEFT:
                x -= speed;
                break;
            case Orientation::RIGHT:
                x += speed;
                break;
            case Orientation::UP:
                y -= speed;
                break;
            case Orientation::DOWN:
                y += speed;
                break;
        }
    }

    x = std::min(std::max(0, x), GameConstants::WINDOW_WIDTH - PACMAN_W);
    y = std::min(std::max(0, y), GameConstants::WINDOW_HEIGHT - PACMAN_H);

    xStartTile = x / 16;
    yStartTile = y / 16;

    // if (orientation == Orientation::LEFT) xStartTile++;
    // if (orientation == Orientation::UP) yStartTile++;

    xEndTile = xStartTile;
    yEndTile = yStartTile;

    switch (orientation) {
        case Orientation::LEFT:  xEndTile = xStartTile - 1; break;
        case Orientation::RIGHT: xEndTile = xStartTile + 1; break;
        case Orientation::UP:    yEndTile = yStartTile - 1; break;
        case Orientation::DOWN:  yEndTile = yStartTile + 1; break;
    }

    // xEndTile = xStartTile;
    // yEndTile = yStartTile;

    // xEndTile = xEndTile > 0 ? xEndTile : 0;
    // yEndTile = yEndTile > 0 ? yEndTile : 0;

    if (animationTimer->getTicks() > 100) {
        animationIndex = (animationIndex + 1) & 1;
        animationTimer->start();
    }
}

void Pacman::render() {
    // std::string textX = "TILE X: " + std::to_string(x / 16) + " X: " + std::to_string(x);
    // std::string textY = "TILE Y: " + std::to_string(y / 16) + " Y: " + std::to_string(y);

    // SDL_Color colorCoordNotOnTile = { 255, 255, 0, 255 };
    // SDL_Color colorCoordOnTile = { 0, 255, 0, 255 };

    // SDL_Surface *textXSurface = TTF_RenderText_Solid(jetbrainsMono, textX.c_str(), x % 16 == 0 ? colorCoordOnTile : colorCoordNotOnTile);
    // SDL_Texture *textXTexture = SDL_CreateTextureFromSurface(renderer, textXSurface);

    // SDL_Surface *textYSurface = TTF_RenderText_Solid(jetbrainsMono, textY.c_str(), y % 16 == 0 ? colorCoordOnTile : colorCoordNotOnTile);
    // SDL_Texture *textYTexture = SDL_CreateTextureFromSurface(renderer, textYSurface);

    // SDL_Rect textXRect = { 0, 0, textXSurface->w, textXSurface->h };
    // SDL_Rect textYRect = { 0, 16, textYSurface->w, textYSurface->h };

    // SDL_RenderCopy(renderer, textXTexture, NULL, &textXRect);
    // SDL_RenderCopy(renderer, textYTexture, NULL, &textYRect);

    // SDL_FreeSurface(textXSurface);
    // SDL_FreeSurface(textYSurface);
    // SDL_DestroyTexture(textXTexture);
    // SDL_DestroyTexture(textYTexture);
   
    SDL_Rect textureRect = { x, y, PACMAN_W, PACMAN_H };
    SDL_Texture *toDraw = animationIndex ? textureUnoriented : textureOriented;

    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    SDL_RenderDrawRect(renderer, &textureRect);
    SDL_RenderCopyEx(renderer, toDraw, NULL, &textureRect, rotationFromOrientation(orientation), NULL, SDL_FLIP_NONE);

    SDL_Rect rect = { x, y, 16, 16 };
    SDL_SetRenderDrawColor(renderer, 255, 51, 51, 255);
    SDL_RenderFillRect(renderer, &rect);

    rect = { xStartTile * 16, yStartTile * 16, 16, 16 };
    SDL_SetRenderDrawColor(renderer, 127, 51, 51, 255);
    SDL_RenderDrawRect(renderer, &rect);

    rect = { xEndTile * 16, yEndTile * 16, 16, 16 };
    SDL_SetRenderDrawColor(renderer, 51, 255, 51, 255);
    SDL_RenderDrawRect(renderer, &rect);

    for (int i = 0; i < 2; i++) {
        SDL_Rect tileRect = {
            (xStartTile + tilesToCheck[desiredOrientation][i].x) * 16,
            (yStartTile + tilesToCheck[desiredOrientation][i].y) * 16,
            16,
            16
        };

        SDL_SetRenderDrawColor(renderer, 51, 51, 255, 255);
        SDL_RenderDrawRect(renderer, &tileRect);
    }

    // SDL_Rect r3 = { tilesToCheck[0].x * 16, tilesToCheck[0].y * 16, 16, 16 };
    // SDL_Rect r4 = { tilesToCheck[1].x * 16, tilesToCheck[1].y * 16, 16, 16 };
    // SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    // SDL_RenderDrawRect(renderer, &r3);
    // SDL_RenderDrawRect(renderer, &r4);
}

int Pacman::rotationFromOrientation(Orientation orientation) {
    switch (orientation) {
        case Orientation::LEFT:  return 180;
        case Orientation::RIGHT: return 0;
        case Orientation::UP:    return 270;
        case Orientation::DOWN:  return 90;
    }
    return 0;
}
