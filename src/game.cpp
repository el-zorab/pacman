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

using GameConst::UNITS_PER_TILE;

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

    ghosts[Ghost::Name::BLINKY] = std::make_unique<Blinky>();
    ghosts[Ghost::Name::PINKY] = std::make_unique<Pinky>();
    ghosts[Ghost::Name::INKY] = std::make_unique<Inky>();
    ghosts[Ghost::Name::CLYDE] = std::make_unique<Clyde>();
    for (auto const &ghost : ghosts) {
        ghost->init();
    }

    ghostsExitMinEatenPellets[Ghost::Name::BLINKY] = 0;
    ghostsExitMinEatenPellets[Ghost::Name::PINKY]  = 0;
    ghostsExitMinEatenPellets[Ghost::Name::INKY]   = 30;
    ghostsExitMinEatenPellets[Ghost::Name::CLYDE]  = 60;

    ghostModes.push_back({ Ghost::Mode::SCATTER, 7000 });
    ghostModes.push_back({ Ghost::Mode::CHASE,   20000 });
    ghostModes.push_back({ Ghost::Mode::SCATTER, 7000 });
    ghostModes.push_back({ Ghost::Mode::CHASE,   20000 });
    ghostModes.push_back({ Ghost::Mode::SCATTER, 5000 });
    ghostModes.push_back({ Ghost::Mode::CHASE,   20000 });
    ghostModes.push_back({ Ghost::Mode::SCATTER, 5000 });
    ghostModes.push_back({ Ghost::Mode::CHASE,   -1 });
    
    currModeIndex = 0;
    currModeTimer = std::make_unique<Timer>();
    currModeTimer->start();

    for (auto const &ghost : ghosts) {
        ghost->setMode(ghostModes[0].mode);
    }

    ghostsFrightened = false;
    ghostsFrightenedTimer = std::make_unique<Timer>();

    ghostsFlashTimer = std::make_unique<Timer>();
    ghostsFlashing = false;

    frameTimeAccumulator = 0.0;
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
    return *ghosts[Ghost::Name::BLINKY];
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
                case SDLK_1:
                    currModeTimer->pause();
                    break;
                case SDLK_2:
                    currModeTimer->unpause();
                    break;
            }
        } else if (event.type == SDL_MOUSEMOTION) {
            SDL_GetMouseState(&mousePos.x, &mousePos.y);
        }
    }
}

void Game::update() {
    frameTimeAccumulator += frameTimer->getMiliseconds();
    frameTimer->start();

    while (frameTimeAccumulator >= PHYSICS_FRAME_DELTA_TIME && gameRunning) {        
        updatePhysicsFrame(PHYSICS_FRAME_DELTA_TIME);
        frameTimeAccumulator -= PHYSICS_FRAME_DELTA_TIME;
    }
}

void Game::updatePhysicsFrame(int const deltaTime) {
    pacman->update(deltaTime);

    PelletType eatenPelletType = pelletManager->removePellet(pacman->getCurrentTile().x, pacman->getCurrentTile().y);
    if (eatenPelletType != PelletType::NONE) {
        if (eatenPelletType == PelletType::ENERGIZER) {
            if (ghostsFrightened) {
                ghostsFlashing = false;
            }
            ghostsFrightened = true;
            ghostsFrightenedTimer->start();
            currModeTimer->pause();

            for (auto const &ghost : ghosts) {
                ghost->setMode(Ghost::Mode::FRIGHTENED);
            }
        }
    }

    if (ghostsFrightened && ghostsFrightenedTimer->getMiliseconds() > GHOST_FRIGHTENED_MS) {
        ghostsFrightened = false;
        ghostsFlashing = false;
        currModeTimer->unpause();

        for (auto const &ghost : ghosts) {
            ghost->setMode(ghostModes[currModeIndex].mode);
        }
    }

    if (ghostsFrightened && !ghostsFlashing && ghostsFrightenedTimer->getMiliseconds() > GHOST_START_FLASHING_MS) {
        ghostsFlashing = true;
        ghostsFlashTimer->start();
    }

    if (ghostsFlashing) {
        if (ghostsFlashTimer->getMiliseconds() < GHOST_FLASHING_INTERVAL_MS) {
            for (auto const &ghost : ghosts) {
                // if (!ghost->isEaten()) {
                    ghost->setGhostColor(GHOST_FLASH_COLOR);
                // }
            }
        } else if (ghostsFlashTimer->getMiliseconds() < 2 * GHOST_FLASHING_INTERVAL_MS) {
            for (auto const &ghost : ghosts) {
                // if (!ghost->isEaten()) {
                    ghost->setGhostColor(GHOST_FRIGHTENED_COLOR);
                // }
            }
        } else {
            ghostsFlashTimer->start();
        }
    }

    if (currModeTimer->getMiliseconds() > ghostModes[currModeIndex].duration && ghostModes[currModeIndex].duration > 0) {
        currModeIndex++;
        currModeTimer->start();

        for (auto const &ghost : ghosts) {
            ghost->setMode(ghostModes[currModeIndex].mode);
        }
    };

    for (int i = 0; i < Ghost::GHOST_COUNT; i++) {
        Ghost::Name ghostName = static_cast<Ghost::Name>(i);
        if (pelletManager->getEatenPellets() >= ghostsExitMinEatenPellets[ghostName]) {
            ghosts[ghostName]->setExitHouse();
        }
    }

    for (auto const &ghost : ghosts) {
        ghost->update(deltaTime);

        if (ghost->getCurrentTile() == pacman->getCurrentTile()) {
            if (ghostsFrightened) {
                ghost->setEaten();
            } else {
                stopRunning();
            }
        }

        if (ghost->isRespawned()) {
            ghost->resetGhostColor();
            ghost->setMode(ghostModes[currModeIndex].mode); // ???
        }
    }
}

void Game::render() {
    SDL_SetRenderDrawColor(renderer, BACKGROUND_COLOR.r, BACKGROUND_COLOR.g, BACKGROUND_COLOR.b, BACKGROUND_COLOR.a);
    SDL_RenderClear(renderer);
    // renderBackground();
    tilingManager->renderTiles();
    renderGhostHouseGate();
    pelletManager->renderPellets();

    pacman->render();
    for (auto const &ghost : ghosts) {
        ghost->render();
    }

    // SDL_Rect mouseTileRect = {
    //     (mousePos.x - GameConst::BORDER_SIZE) / GameConst::TILE_SIZE * GameConst::TILE_SIZE,
    //     (mousePos.y - GameConst::BORDER_SIZE) / GameConst::TILE_SIZE * GameConst::TILE_SIZE,
    //     GameConst::TILE_SIZE,
    //     GameConst::TILE_SIZE
    // };
    // SDL_SetRenderDrawColor(renderer, TEXT_COLOR.r, TEXT_COLOR.g, TEXT_COLOR.b, TEXT_COLOR.a);
    // SDL_RenderFillRect(renderer, &mouseTileRect);

    // renderText("X=" + std::to_string(mouseTileRect.x / GameConst::TILE_SIZE), 0, 0, TEXT_COLOR);
    // renderText("Y=" + std::to_string(mouseTileRect.y / GameConst::TILE_SIZE), 0, GameConst::TILE_SIZE, TEXT_COLOR);

    if (ghostsFrightened) {
        renderText("MODE_INDEX=FRIGHTENED [" + std::to_string(ghostsFrightenedTimer->getMiliseconds()) + "]", 0, 2 * GameConst::TILE_SIZE, GHOST_FRIGHTENED_COLOR);
    } else if (ghostModes[currModeIndex].mode == Ghost::Mode::CHASE) {
        renderText("MODE_INDEX=CHASE [" + std::to_string(currModeTimer->getMiliseconds()) + "]", 0, 2 * GameConst::TILE_SIZE, {255, 127, 127, 255});
    } else if (ghostModes[currModeIndex].mode == Ghost::Mode::SCATTER) {
        renderText("MODE_INDEX=SCATTER [" + std::to_string(currModeTimer->getMiliseconds()) + "]", 0, 2 * GameConst::TILE_SIZE, {127, 127, 255, 255});
    }
    renderText("EAT_PELL=" + std::to_string(pelletManager->getEatenPellets()), 0, 3 * GameConst::TILE_SIZE, {255, 255, 255, 255});

    SDL_RenderPresent(renderer);
}

void Game::renderBackground() {   
    for (int i = 0; i < GameConst::TILE_COLS; i++) {
        for (int j = 0; j < GameConst::TILE_ROWS; j++) {
            SDL_Rect tileRect = {
                GameConst::TILE_SIZE * i,
                GameConst::TILE_SIZE * j,
                GameConst::TILE_SIZE,
                GameConst::TILE_SIZE
            };

            SDL_Color tileColor = (i + j) % 2 == 0 ? TILE_COLOR_A : TILE_COLOR_B;
            SDL_SetRenderDrawColor(renderer, tileColor.r, tileColor.g, tileColor.b, tileColor.a);
            SDL_RenderFillRect(renderer, &tileRect);
        }
    }
}

void Game::renderGhostHouseGate() {
    SDL_Rect ghostHouseGateRect = {
        GHOST_HOUSE_TILE.x * GameConst::TILE_SIZE,
        GHOST_HOUSE_TILE.y * GameConst::TILE_SIZE + GameConst::TILE_SIZE / 2 - GHOST_HOUSE_GATE_HEIGHT / 2,
        GHOST_HOUSE_GATE_WIDTH,
        GHOST_HOUSE_GATE_HEIGHT
    };
    SDL_SetRenderDrawColor(renderer, GHOST_HOUSE_GATE_COLOR.r, GHOST_HOUSE_GATE_COLOR.g, GHOST_HOUSE_GATE_COLOR.b, GHOST_HOUSE_GATE_COLOR.a);
    SDL_RenderFillRect(renderer, &ghostHouseGateRect);
}
