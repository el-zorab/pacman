#include <cmath>
#include <fstream>
#include <stdio.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "blinky.hpp"
#include "clyde.hpp"
#include "game.hpp"
#include "inky.hpp"
#include "pinky.hpp"
#include "textureManager.hpp"
#include "tilingManager.hpp"

Game::Game(_ctr_tag) {}

Game &Game::getInstance() {
    static std::unique_ptr<Game> instance = std::make_unique<Game>(_ctr_tag{});
    return *instance;
}

void Game::init(std::string title, int x, int y) {
    window = SDL_CreateWindow(title.c_str(), x, y, GameConst::WINDOW_BORDERED_WIDTH, GameConst::WINDOW_BORDERED_HEIGHT, SDL_WINDOW_SHOWN);
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

    SDL_Rect rendererViewport = { GameConst::BORDER_SIZE, GameConst::BORDER_SIZE, GameConst::WINDOW_WIDTH, GameConst::WINDOW_HEIGHT };
    SDL_RenderSetViewport(renderer, &rendererViewport);

    gameRunning = true;
    
    fontRenderer = std::make_unique<FontRenderer>("jetbrains_mono_regular.ttf", GameConst::TILE_SIZE);
    textureManager = std::make_unique<TextureManager>();
    tilingManager = std::make_unique<TilingManager>();
    pelletManager = std::make_unique<PelletManager>();

    pacman = std::make_unique<Pacman>();

    ghosts[GhostName::BLINKY] = std::make_unique<Blinky>();
    ghosts[GhostName::PINKY] = std::make_unique<Pinky>();
    ghosts[GhostName::INKY] = std::make_unique<Inky>();
    ghosts[GhostName::CLYDE] = std::make_unique<Clyde>();

    ghosts[GhostName::BLINKY]->init({10, 14}, Orientation::RIGHT, Ghost::State::SCATTER);
    ghosts[GhostName::PINKY]->init({12, 14}, Orientation::RIGHT, Ghost::State::SCATTER);
    ghosts[GhostName::INKY]->init({15, 14}, Orientation::RIGHT, Ghost::State::SCATTER);
    ghosts[GhostName::CLYDE]->init({16, 14}, Orientation::RIGHT, Ghost::State::SCATTER);

    frameAccumulator = 0.0;
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

SDL_Renderer *Game::getRenderer() {
    return renderer;
};

TextureManager &Game::getTextureManager() {
    return *textureManager;
}

TilingManager &Game::getTilingManager() {
    return *tilingManager;
}

Pacman &Game::getPacman() {
    return *pacman;
}
Ghost &Game::getBlinky() {
    return *ghosts[GhostName::BLINKY];
}

void Game::renderText(std::string text, int x, int y, SDL_Color color) {
    fontRenderer->renderText(renderer, text, x, y, color);
}

void Game::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            stopRunning();
        } else if (event.type == SDL_KEYDOWN) {

            switch (event.key.keysym.sym) {
                case SDLK_LEFT:
                    pacman->setDesiredOrientation(Orientation::LEFT);
                    break;
                case SDLK_RIGHT:
                case SDLK_d:
                    pacman->setDesiredOrientation(Orientation::RIGHT);
                case SDLK_a:
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
    float frameTime = frameTimer->getTicks();
    frameTimer->start();

    frameAccumulator += frameTime;

    const int SIMULATION_FRAMERATE = 60;
    const double dt = 1000.f / SIMULATION_FRAMERATE;
    const double dt_floored = std::floor(dt);

    while (frameAccumulator >= dt_floored && gameRunning) {        
        pacman->update(dt_floored);
        for (auto const &ghost : ghosts) {
            ghost->update(dt_floored);
            
            if (ghost->getCurrentTile() == pacman->getCurrentTile()) {
                // SDL_Log("You died!\n");
                // stopRunning();
            }
        }

        pelletManager->removePellet(pacman->getCurrentTile().x, pacman->getCurrentTile().y);

        frameAccumulator -= dt_floored;
    }
}

void Game::render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    renderBackground();
    tilingManager->renderTiles();
    pelletManager->renderPellets();

    for (auto const &ghost : ghosts) {
        ghost->render();
    }
    pacman->render();

    SDL_RenderPresent(renderer);
}

void Game::renderBackground() {
    const SDL_Color BG_TILE_COLOR_A = { 15, 15, 15, 255 };
    const SDL_Color BG_TILE_COLOR_B = { 25, 25, 25, 255 };

    // const SDL_Color BG_TILE_COLOR_A = { 51, 51, 51, 255 };
    // const SDL_Color BG_TILE_COLOR_B = { 100, 100, 100, 255 };
    
    for (int i = 0; i < GameConst::TILE_COLS; i++) {
        for (int j = 0; j < GameConst::TILE_ROWS; j++) {
            SDL_Rect tileRect = {
                GameConst::TILE_SIZE * i,
                GameConst::TILE_SIZE * j,
                GameConst::TILE_SIZE,
                GameConst::TILE_SIZE
            };

            SDL_Color color = (i + j) % 2 == 0 ? BG_TILE_COLOR_A : BG_TILE_COLOR_B;
            SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
            SDL_RenderFillRect(renderer, &tileRect);
        }
    }
}
