#include <stdio.h>

#include <SDL2/SDL_image.h>

#include "game.hpp"
#include "gameConstants.hpp"

Game::Game() {
    window = nullptr;
    windowWidth = 0;
    windowHeight = 0;

    renderer = nullptr;

    pacman = nullptr;

    gameRunning = false;
}

void Game::init(std::string title, int x, int y) {
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

    pacman = std::make_unique<Pacman>(); 
    pacman->init(renderer);

    gameRunning = true;
}

void Game::close() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    renderer = nullptr;
    window = nullptr;

    SDL_Quit();
}

bool Game::isGameRunning() {
    return gameRunning;
}

void Game::stopRunning() {
    gameRunning = false;
}

void Game::handleKeyboardEvents() {
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

void Game::update() {
    pacman->update();
}

void Game::render() {
    Game::renderBackground(); // this will also clear the window

    pacman->render();

    SDL_RenderPresent(renderer);

    SDL_Delay(300);
}

void Game::renderBackground() {
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
