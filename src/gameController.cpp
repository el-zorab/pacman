#include <stdio.h>

#include <SDL2/SDL_image.h>

#include "gameConstants.hpp"
#include "gameController.hpp"
#include "texture.hpp"

SDL_Texture *pacman = nullptr;

GameController::GameController() {
    window = nullptr;
    windowWidth = 0;
    windowHeight = 0;

    renderer = nullptr;

    gameRunning = false;
}

void GameController::init(std::string title, int x, int y) {
    windowWidth  = GameConstants::HORIZONTAL_TILES * GameConstants::TILE_SIZE;
    windowHeight = GameConstants::VERTICAL_TILES   * GameConstants::TILE_SIZE;

    window = SDL_CreateWindow(title.c_str(), x, y, windowWidth, windowHeight, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        printf("SDL_CreateWindow error: %s\n", SDL_GetError());
        exit(-1);
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr) {
        printf("SDL_CreateRenderer error: %s\n", SDL_GetError());
        exit(-1);
    }

    int IMG_INIT_FLAGS = IMG_INIT_PNG;
    if (!(IMG_Init(IMG_INIT_FLAGS) & IMG_INIT_FLAGS)) {
        printf("IMG_Init error: %s\n", IMG_GetError());
        exit(-1);
    }

    pacman = loadTexture(renderer, "pacman.png");

    gameRunning = true;
}

void GameController::close() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    renderer = nullptr;
    window = nullptr;

    SDL_Quit();
}

bool GameController::isGameRunning() {
    return gameRunning;
}

void GameController::stopRunning() {
    gameRunning = false;
}

void GameController::handleKeyboardEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            stopRunning();
        } else if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
                case SDLK_UP:
                    printf("up\n");
                    break;
            }
        }
    }
}

int rotation = 0;

void GameController::update() {
    rotation++;
    rotation &= 3;
}

void GameController::render() {
    GameController::renderBackground(); // this will also clear the window

    SDL_Rect destrect = {
        3 * GameConstants::TILE_SIZE + GameConstants::TILE_SIZE / 2,
        4 * GameConstants::TILE_SIZE + GameConstants::TILE_SIZE / 2,
        32,
        32
    };

    SDL_RenderCopyEx(renderer, pacman, NULL, &destrect, rotation * 90, NULL, SDL_FLIP_NONE);
    SDL_RenderPresent(renderer);

    SDL_Delay(300);
}

void GameController::renderBackground() {
    const int BG_TILE_GRAYSCALE_A = 0;
    const int BG_TILE_GRAYSCALE_B = 20;

    for (int i = 0; i < GameConstants::HORIZONTAL_TILES; i++) {
        for (int j = 0; j < GameConstants::VERTICAL_TILES; j++) {
            SDL_Rect tileRect = {
                GameConstants::TILE_SIZE * i,
                GameConstants::TILE_SIZE * j,
                GameConstants::TILE_SIZE,
                GameConstants::TILE_SIZE
            };

            if ((i + j) % 2 == 0) {
                SDL_SetRenderDrawColor(renderer, BG_TILE_GRAYSCALE_A, BG_TILE_GRAYSCALE_A, BG_TILE_GRAYSCALE_A, 255);
            } else {
                SDL_SetRenderDrawColor(renderer, BG_TILE_GRAYSCALE_B, BG_TILE_GRAYSCALE_B, BG_TILE_GRAYSCALE_B, 255);
            }

            SDL_RenderFillRect(renderer, &tileRect);
        }
    }
}
