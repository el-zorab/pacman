#include <cmath>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "gameConstants.hpp"
#include "pacman.hpp"
#include "textureLoader.hpp"

TTF_Font *jetbrainsMono = nullptr;

static const int PACMAN_W = 32;
static const int PACMAN_H = 32;

static const std::string PACMAN_TEXTURE_ORIENTED_PATH = "res/pacman_oriented.png";
static const std::string PACMAN_TEXTURE_UNORIENTED_PATH = "res/pacman_unoriented.png";

Pacman::Pacman(SDL_Renderer *renderer) {
    this->renderer = renderer;

    textureOriented = loadTexture(renderer, PACMAN_TEXTURE_ORIENTED_PATH, true);
    textureUnoriented = loadTexture(renderer, PACMAN_TEXTURE_UNORIENTED_PATH, true);

    orientation = PacmanOrientation::RIGHT;

    x = 0;
    y = 0;

    animationTimer = std::make_unique<Timer>();
    animationIndex = 0;

    jetbrainsMono = TTF_OpenFont("res/JetBrainsMono-Regular.ttf", 16);

    animationTimer->start();
}

void Pacman::close() {
    TTF_CloseFont(jetbrainsMono);
    SDL_DestroyTexture(textureOriented);
    SDL_DestroyTexture(textureUnoriented);
    textureOriented = nullptr;
    textureUnoriented = nullptr;
}

void Pacman::setDesiredOrientation(PacmanOrientation desiredOrientation) {
    this->orientation = desiredOrientation;
}

void Pacman::setOrientation(PacmanOrientation orientation) {
    this->orientation = orientation;
}

void Pacman::update(float deltaTime) {
    (void) deltaTime;

    int speed = 1;
    switch (orientation) {
        case PacmanOrientation::LEFT:
            x -= speed;
            break;
        case PacmanOrientation::RIGHT:
            x += speed;
            break;
        case PacmanOrientation::UP:
            y -= speed;
            break;
        case PacmanOrientation::DOWN:
            y += speed;
            break;
    }

    if (animationTimer->getTicks() > 100) {
        animationIndex = (animationIndex + 1) & 1;
        animationTimer->start();
    }

    x = std::min(std::max(0, x), GameConstants::WINDOW_WIDTH - PACMAN_W);
    y = std::min(std::max(0, y), GameConstants::WINDOW_HEIGHT - PACMAN_H);
}

void Pacman::render() {
    SDL_Rect textureRect = { x, y, PACMAN_W, PACMAN_H };

    SDL_Texture *toDraw = animationIndex ? textureUnoriented : textureOriented;

    int rotation;

    switch (orientation) {
        case PacmanOrientation::LEFT:
            rotation = 180;
            break;
        case PacmanOrientation::RIGHT:
            rotation = 0;
            break;
        case PacmanOrientation::UP:
            rotation = 270;
            break;
        case PacmanOrientation::DOWN:
            rotation = 90;
            break;
    }

    std::string textX = "TILE X: " + std::to_string(x / 16) + " X: " + std::to_string(x);
    std::string textY = "TILE Y: " + std::to_string(y / 16) + " Y: " + std::to_string(y);

    SDL_Color colorCoordNotOnTile = { 255, 255, 0, 255 };
    SDL_Color colorCoordOnTile = { 0, 255, 0, 255 };

    SDL_Surface *textXSurface = TTF_RenderText_Solid(jetbrainsMono, textX.c_str(), x % 16 == 0 ? colorCoordOnTile : colorCoordNotOnTile);
    SDL_Texture *textXTexture = SDL_CreateTextureFromSurface(renderer, textXSurface);

    SDL_Surface *textYSurface = TTF_RenderText_Solid(jetbrainsMono, textY.c_str(), y % 16 == 0 ? colorCoordOnTile : colorCoordNotOnTile);
    SDL_Texture *textYTexture = SDL_CreateTextureFromSurface(renderer, textYSurface);

    SDL_Rect textXRect = { 0, 0, textXSurface->w, textXSurface->h };
    SDL_Rect textYRect = { 0, 16, textYSurface->w, textYSurface->h };

    SDL_RenderCopy(renderer, textXTexture, NULL, &textXRect);
    SDL_RenderCopy(renderer, textYTexture, NULL, &textYRect);

    SDL_FreeSurface(textXSurface);
    SDL_FreeSurface(textYSurface);
    SDL_DestroyTexture(textXTexture);
    SDL_DestroyTexture(textYTexture);

    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    SDL_RenderDrawRect(renderer, &textureRect);
    SDL_RenderCopyEx(renderer, toDraw, NULL, &textureRect, rotation, NULL, SDL_FLIP_NONE);
}
