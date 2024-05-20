#include <fstream>
#include <stdio.h>
#include <SDL2/SDL_image.h>

#include "game.hpp"
#include "gameConstants.hpp"

bool tiles[GameConstants::TILE_ROWS - 1][GameConstants::TILE_COLS - 1];

Game::Game(std::string title, int x, int y) {
    window = SDL_CreateWindow(title.c_str(), x, y, GameConstants::WINDOW_WIDTH, GameConstants::WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
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

    gameRunning = true;

    frameTimer = std::make_unique<Timer>();

    pacman = std::make_unique<Pacman>(renderer);

    std::ifstream tilingFile("res/tiling.dat");

    for (int i = 0; i < GameConstants::TILE_ROWS - 1; i++) {
        for (int j = 0; j < GameConstants::TILE_COLS - 1; j++) {
            char c;
            tilingFile >> c;
            tiles[i][j] = int(c) - '0';
            printf("%d ", (int) tiles[i][j]);
        }
        printf("\n");
    }

    frameTimer->start();
}

Game::~Game() {
    close();
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
                case SDLK_LEFT:
                    pacman->setOrientation(PacmanOrientation::LEFT);
                    break;
                case SDLK_RIGHT:
                    pacman->setOrientation(PacmanOrientation::RIGHT);
                    break;
                case SDLK_UP:
                    pacman->setOrientation(PacmanOrientation::UP);
                    break;
                case SDLK_DOWN:
                    pacman->setOrientation(PacmanOrientation::DOWN);
                    break;
            }
        }
    }
}

void Game::update() {
    float deltaTime = frameTimer->getTicks() / 1000.f;
    frameTimer->start();

    pacman->update(deltaTime);
}

void Game::render() {
    // rendering the background will also clear the window
    renderBackground();

    SDL_SetRenderDrawColor(renderer, 127, 127, 127, 255);

    SDL_RenderDrawLine(renderer, GameConstants::WINDOW_WIDTH / 2, 0,
                                 GameConstants::WINDOW_WIDTH / 2, GameConstants::WINDOW_HEIGHT);

    for (int i = 0; i < GameConstants::TILE_ROWS - 1; i++) {
        for (int j = 0; j < GameConstants::TILE_COLS - 1; j++) {
            SDL_Rect tileRect = {
                GameConstants::TILE_SIZE * j + 8,
                GameConstants::TILE_SIZE * i + 8,
                GameConstants::TILE_SIZE,
                GameConstants::TILE_SIZE
            };

            if (tiles[i][j]) {
                SDL_RenderDrawRect(renderer, &tileRect);
            }
        }
    }

    pacman->render();

    SDL_RenderPresent(renderer);
}

void Game::renderBackground() {
    const int BG_TILE_GRAYSCALE_A = 0;
    const int BG_TILE_GRAYSCALE_B = 25;

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    
    for (int i = 0; i < GameConstants::TILE_ROWS; i++) {
        for (int j = 0; j < GameConstants::TILE_COLS; j++) {
            SDL_Rect tileRect = {
                GameConstants::TILE_SIZE * j,
                GameConstants::TILE_SIZE * i,
                GameConstants::TILE_SIZE,
                GameConstants::TILE_SIZE
            };

            // if (tiles[i][j]) {
            //     SDL_SetRenderDrawColor(renderer, 127, 0, 0, 255);
            // } else {
                if ((i + j) % 2 == 0) {
                    SDL_SetRenderDrawColor(renderer, BG_TILE_GRAYSCALE_A, BG_TILE_GRAYSCALE_A, BG_TILE_GRAYSCALE_A, 255);
                } else {
                    SDL_SetRenderDrawColor(renderer, BG_TILE_GRAYSCALE_B, BG_TILE_GRAYSCALE_B, BG_TILE_GRAYSCALE_B, 255);
                }
            // }

            SDL_RenderFillRect(renderer, &tileRect);
        }
    }
}
