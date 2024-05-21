#include <fstream>
#include <stdio.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "game.hpp"
#include "gameConstants.hpp"
#include "tilingManager.hpp"

bool tiles[GameConstants::TILE_ROWS][GameConstants::TILE_COLS];

Game::Game(std::string title, int x, int y) {
    window = SDL_CreateWindow(title.c_str(), x, y, GameConstants::WINDOW_BORDERED_WIDTH, GameConstants::WINDOW_BORDERED_HEIGHT, SDL_WINDOW_SHOWN);
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

    if (TTF_Init() == -1) {
        printf("TTF_Init error: %s\n", TTF_GetError());
        exit(-1);
    }

    SDL_Rect rendererViewport = {
        GameConstants::BORDER_SIZE, GameConstants::BORDER_SIZE,
        GameConstants::WINDOW_WIDTH, GameConstants::WINDOW_HEIGHT
    };

    SDL_RenderSetViewport(renderer, &rendererViewport);

    gameRunning = true;

    TilingManager::getInstance().loadTiling();

    pacman = std::make_unique<Pacman>(renderer);

    frameTimer = std::make_unique<Timer>();
    frameTimer->start();
}

void Game::close() {
    pacman->close();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    renderer = nullptr;
    window = nullptr;

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

bool Game::isGameRunning() {
    return gameRunning;
}

void Game::stopRunning() {
    gameRunning = false;
}

void Game::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            stopRunning();
        } else if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
                case SDLK_LEFT:
                case SDLK_a:
                    pacman->setDesiredOrientation(Pacman::Orientation::LEFT);
                    break;
                case SDLK_RIGHT:
                case SDLK_d:
                    pacman->setDesiredOrientation(Pacman::Orientation::RIGHT);
                    break;
                case SDLK_UP:
                case SDLK_w:
                    pacman->setDesiredOrientation(Pacman::Orientation::UP);
                    break;
                case SDLK_DOWN:
                case SDLK_s:
                    pacman->setDesiredOrientation(Pacman::Orientation::DOWN);
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
    SDL_SetRenderDrawColor(renderer, 31, 31, 127, 255);
    SDL_RenderClear(renderer);

    renderBackgroundTiling();
    renderTiling();

    pacman->render();

    SDL_RenderPresent(renderer);
}

void Game::renderBackgroundTiling() {
    SDL_Color BG_TILE_COLOR_A = { 0, 0, 0, 255 };
    SDL_Color BG_TILE_COLOR_B = { 25, 25, 25, 255 };
    
    for (int i = 0; i < GameConstants::TILE_COLS; i++) {
        for (int j = 0; j < GameConstants::TILE_ROWS; j++) {
            SDL_Rect tileRect = {
                GameConstants::TILE_SIZE * i,
                GameConstants::TILE_SIZE * j,
                GameConstants::TILE_SIZE,
                GameConstants::TILE_SIZE
            };

            SDL_Color color = (i + j) % 2 == 0 ? BG_TILE_COLOR_A : BG_TILE_COLOR_B;
            SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
            SDL_RenderFillRect(renderer, &tileRect);
        }
    }
}

void Game::renderTiling() {
    SDL_SetRenderDrawColor(renderer, 127, 127, 127, 255);

    SDL_RenderDrawLine(renderer,
        GameConstants::WINDOW_WIDTH / 2, 0,
        GameConstants::WINDOW_WIDTH / 2, GameConstants::WINDOW_HEIGHT);

    for (int i = 0; i < GameConstants::TILE_COLS; i++) {
        for (int j = 0; j < GameConstants::TILE_ROWS; j++) {
            if (!TilingManager::getInstance().getTilingAt(i, j)) continue;

            SDL_Rect tileRect = {
                GameConstants::TILE_SIZE * i,
                GameConstants::TILE_SIZE * j,
                GameConstants::TILE_SIZE,
                GameConstants::TILE_SIZE
            };

            SDL_RenderDrawRect(renderer, &tileRect);
        }
    }
}
