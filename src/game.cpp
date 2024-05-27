#include <fstream>
#include <stdio.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "game.hpp"
#include "gameConstants.hpp"
#include "textureManager.hpp"
#include "tilingManager.hpp"

Game::Game(_ctr_tag){}

Game &Game::getInstance() {
    static std::unique_ptr<Game> instance = std::make_unique<Game>(_ctr_tag{});
    return *instance;
}

void Game::init(std::string title, int x, int y) {
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
    
    fontRenderer = std::make_unique<FontRenderer>("jetbrains_mono_regular.ttf", GameConstants::TILE_SIZE);
    textureManager = std::make_unique<TextureManager>();
    tilingManager = std::make_unique<TilingManager>();
    tilingManager->loadTiling();

    pacman = std::make_unique<Pacman>(renderer);

    frameTimer = std::make_unique<Timer>();
    frameTimer->start();
}

void Game::close() {
    fontRenderer->close();
    textureManager->close();

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

FontRenderer &Game::getFontRenderer() {
    return *fontRenderer;
}

TextureManager &Game::getTextureManager() {
    return *textureManager;
}

TilingManager &Game::getTilingManager() {
    return *tilingManager;
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
                    pacman->setDesiredOrientation(Orientation::LEFT);
                    break;
                case SDLK_RIGHT:
                case SDLK_d:
                    pacman->setDesiredOrientation(Orientation::RIGHT);
                    break;
                case SDLK_UP:
                case SDLK_w:
                    pacman->setDesiredOrientation(Orientation::UP);
                    break;
                case SDLK_DOWN:
                case SDLK_s:
                    pacman->setDesiredOrientation(Orientation::DOWN);
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
    renderBackground();
    renderMap();

    pacman->render();

    SDL_RenderPresent(renderer);
}

void Game::renderBackground() {
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

void Game::renderMap() {
    const int offset = 1;

    SDL_SetRenderDrawColor(renderer, 52, 110, 235, 255);
    for (int i = 0; i < GameConstants::TILE_COLS; i++) {
        for (int j = 0; j < GameConstants::TILE_ROWS; j++) {
            if (tilingManager->isTileFree(i, j)) continue;

            SDL_Rect tileRect = {
                GameConstants::TILE_SIZE * i + offset,
                GameConstants::TILE_SIZE * j + offset,
                GameConstants::TILE_SIZE - 2 * offset,
                GameConstants::TILE_SIZE - 2 * offset
            };

            SDL_RenderFillRect(renderer, &tileRect);
        }
    }
}
