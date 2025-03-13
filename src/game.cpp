#include <cmath>
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

    gameState = GameState::WAITING_FOR_START;
    gameStateWaitTimer = std::make_unique<Timer>();
    gameStateWaitTimer->start();

    gameHighscoreFileIn.open("res/highscore.dat");
    gameHighscoreFileIn >> gameHighscore;
    gameHighscoreFileIn.close();

    gameHighscoreFileOut.open("res/highscore.dat");

    fontRenderer = std::make_unique<FontRenderer>("font.ttf", 3 * GameConst::TILE_SIZE / 2);
    textureManager = std::make_unique<TextureManager>();
    tilingManager = std::make_unique<TilingManager>();
    pelletManager = std::make_unique<PelletManager>();
    pelletManager->init();

    waitingForStartTexture = textureManager->loadTexture("waiting_for_start.png");
    gameLostTexture = textureManager->loadTexture("game_lost.png");

    pacman = std::make_unique<Pacman>();

    ghosts[Ghost::Name::BLINKY] = std::make_unique<Blinky>();
    ghosts[Ghost::Name::PINKY] = std::make_unique<Pinky>();
    ghosts[Ghost::Name::INKY] = std::make_unique<Inky>();
    ghosts[Ghost::Name::CLYDE] = std::make_unique<Clyde>();

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
    
    currModeTimer = std::make_unique<Timer>();

    ghostsFrightenedTimer = std::make_unique<Timer>();

    ghostsFlashingTimer = std::make_unique<Timer>();
    ghostsFlashingTimer->start();

    frameTimeAccumulator = 0.0;
    frameTimer = std::make_unique<Timer>();
    frameTimer->start();
}

void Game::startLevel() {
    pacman->reset();
    for (auto const &ghost : ghosts) {
        ghost->init();
    }

    ghostModeCurr = 0;
    currModeTimer->start();

    for (auto const &ghost : ghosts) {
        ghost->setMode(ghostModes[0].mode);
    }

    ghostsFrightened = false;

    frameTimeAccumulator = 0.0;
    frameTimer->start();
}

void Game::startNewLevel() {
    pelletManager->init();
    startLevel();
}

void Game::startGame() {
    pelletManager->init();
    remainingLives = GAME_LIVES - 1;
    gameScore = 0;
    gameLevel = 1;
    gameStateWaiting = false;
    gameStateWaitMs = 0;
    startNewLevel();
}

void Game::close() {
    gameHighscoreFileOut << gameHighscore;
    gameHighscoreFileOut.close();

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

bool Game::shouldQuit() {
    return gameState == GameState::QUIT;
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
            gameState = GameState::QUIT;
        } else if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
                case SDLK_ESCAPE:
                    gameState = GameState::QUIT;
                    return;
                case SDLK_RETURN:
                    if (gameState == GameState::WAITING_FOR_START) {
                        gameState = GameState::RUNNING;
                        startGame();
                    }
                    break;
                case SDLK_LEFT:
                case SDLK_a:
                    if (gameState == GameState::RUNNING) pacman->setDesiredOrientation(Orientation::LEFT);
                    break;
                case SDLK_RIGHT:
                case SDLK_d:
                    if (gameState == GameState::RUNNING) pacman->setDesiredOrientation(Orientation::RIGHT);
                    break;
                case SDLK_UP:
                case SDLK_w:
                    if (gameState == GameState::RUNNING) pacman->setDesiredOrientation(Orientation::UP);
                    break;
                case SDLK_DOWN:
                case SDLK_s:
                    if (gameState == GameState::RUNNING) pacman->setDesiredOrientation(Orientation::DOWN);
                    break;
            }
        }
    }
}

void Game::update() {
    frameTimeAccumulator += std::min(frameTimer->getMiliseconds(), 4 * PHYSICS_FRAME_DELTA_TIME);
    frameTimer->start();

    while (frameTimeAccumulator >= PHYSICS_FRAME_DELTA_TIME) {
        if (gameState != GameState::RUNNING) {
            updateFrame();
            frameTimer->pause();
        } else {
            updateRunningFrame(PHYSICS_FRAME_DELTA_TIME);
            frameTimer->unpause();
        }

        frameTimeAccumulator -= PHYSICS_FRAME_DELTA_TIME;
    }
}

void Game::updateFrame() {
    if (gameStateWaitMs != 0) {
        if (!gameStateWaiting) {
            gameStateWaiting = true;
            gameStateWaitTimer->start();
        }

        if (gameStateWaitTimer->getMiliseconds() < gameStateWaitMs) {
            return;
        } else {
            gameStateWaitMs = 0;
            gameStateWaiting = false;
        }
    }

    if (gameState == GameState::WIN_LEVEL) {
        gameLevel++;
        gameState = GameState::RUNNING;
        startNewLevel();
    } else if (gameState == GameState::LOST_LIFE) {
        remainingLives--;
        gameState = GameState::RUNNING;
        startLevel();
    } else if (gameState == GameState::LOST_GAME) {
        if (gameScore > gameHighscore) {
            gameHighscore = gameScore;
        }
        gameState = GameState::WAITING_FOR_START;
        startGame();
    }
}

void Game::updateRunningFrame(int const deltaTime) {
    pacman->update(deltaTime);

    PelletType eatenPelletType = pelletManager->removePellet(pacman->getCurrentTile().x, pacman->getCurrentTile().y);
    if (eatenPelletType != PelletType::NONE) {
        if (eatenPelletType == PelletType::ENERGIZER) {
            if (!ghostsFrightened) {
                ghostEatenPoints = GHOST_EATEN_POINTS_BASE;
            }
            ghostsFrightened = true;
            ghostsFrightenedTimer->start();
            ghostsFlashingTimer->start();
            currModeTimer->pause();
            pacman->setGhostsFrightened(true);

            for (auto const &ghost : ghosts) {
                ghost->setMode(Ghost::Mode::FRIGHTENED);
            }

            gameScore += GAME_SCORE_ENERGIZER;
        } else if (eatenPelletType == PelletType::PELLET) {
            gameScore += GAME_SCORE_PELLET;
        }
    }

    if (ghostsFrightened && ghostsFrightenedTimer->getMiliseconds() > GHOST_FRIGHTENED_MS) {
        ghostsFrightened = false;
        currModeTimer->unpause();
        pacman->setGhostsFrightened(false);

        for (auto const &ghost : ghosts) {
            ghost->setMode(ghostModes[ghostModeCurr].mode);
        }
    }

    if (ghostsFrightened && ghostsFrightenedTimer->getMiliseconds() > GHOST_START_FLASHING_MS) {
        for (auto const &ghost : ghosts) {
            if (ghost->isFrightened() && !ghost->isEaten()) {
                if (ghostsFlashingTimer->getMiliseconds() < GHOST_FLASHING_INTERVAL_MS) {
                    ghost->setGhostFlashColor();
                } else if (ghostsFlashingTimer->getMiliseconds() < 2 * GHOST_FLASHING_INTERVAL_MS) {
                    ghost->setGhostFrightenedColor();
                } else {
                    ghostsFlashingTimer->start();
                }
            }
        }
    }

    if (currModeTimer->getMiliseconds() > ghostModes[ghostModeCurr].duration && ghostModes[ghostModeCurr].duration > 0) {
        ghostModeCurr++;
        currModeTimer->start();

        for (auto const &ghost : ghosts) {
            ghost->setMode(ghostModes[ghostModeCurr].mode);
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

        Entity2D deltaPos = ghost->getCurrentPos() - pacman->getCurrentPos();
        if (std::abs(deltaPos.x) < UNITS_PER_TILE && std::abs(deltaPos.y) < UNITS_PER_TILE) {
            if (ghost->isFrightened()) {
                if (!ghost->isEaten()) {
                    ghost->setEaten();
                    gameScore += ghostEatenPoints;
                    ghostEatenPoints *= GHOST_EATEN_POINTS_MUL;
                }
            } else {
                if (remainingLives == 0) {
                    gameState = GameState::LOST_GAME;
                } else {
                    gameState = GameState::LOST_LIFE;
                }
            }
        }

        if (ghost->isRespawned()) {
            ghost->setMode(ghostModes[ghostModeCurr].mode);
        }
    }

    if (pelletManager->getRemainingPellets() == 0) {
        gameState = GameState::WIN_LEVEL;
    }
}

void Game::render() {
    renderClear();

    if (gameState == GameState::WAITING_FOR_START) {
        SDL_RenderCopy(renderer, waitingForStartTexture, nullptr, nullptr);

        std::string highscoreText = "HIGHSCORE: " + std::to_string(gameHighscore);
        int textWidth, textHeight;
        TTF_SizeText(fontRenderer->getFont(), highscoreText.c_str(), &textWidth, &textHeight);
        renderText(highscoreText, (GameConst::WINDOW_WIDTH - textWidth) / 2, 12 * GameConst::TILE_SIZE, TEXT_COLOR);

        SDL_RenderPresent(renderer);

    } else if (gameState == GameState::RUNNING) {
        renderMap();
        pacman->render();
        for (auto const &ghost : ghosts) {
            ghost->render();
        }
        SDL_RenderPresent(renderer);

    } else if (gameState == GameState::LOST_LIFE) {
        renderMap();
        pacman->render();
        for (auto const &ghost : ghosts) {
            Entity2D deltaPos = ghost->getCurrentPos() - pacman->getCurrentPos();
            if (std::abs(deltaPos.x) < UNITS_PER_TILE && std::abs(deltaPos.y) < UNITS_PER_TILE) {
                ghost->render();
                break;
            }
        }
        SDL_RenderPresent(renderer);
        gameStateWaitMs = 2000;

    } else if (gameState == GameState::LOST_GAME) {
        SDL_RenderCopy(renderer, gameLostTexture, nullptr, nullptr);

        std::string scoreText = "SCORE: " + std::to_string(gameScore);
        int textWidth, textHeight;
        TTF_SizeText(fontRenderer->getFont(), scoreText.c_str(), &textWidth, &textHeight);
        renderText(scoreText, (GameConst::WINDOW_WIDTH - textWidth) / 2, 12 * GameConst::TILE_SIZE, TEXT_COLOR);

        SDL_RenderPresent(renderer);
        gameStateWaitMs = 3000;
    }
}

void Game::renderClear() {
    SDL_SetRenderDrawColor(renderer, BACKGROUND_COLOR.r, BACKGROUND_COLOR.g, BACKGROUND_COLOR.b, BACKGROUND_COLOR.a);
    SDL_RenderClear(renderer);
}

void Game::renderGhostHouseGate() {
    SDL_SetRenderDrawColor(renderer, GHOST_HOUSE_GATE_COLOR.r, GHOST_HOUSE_GATE_COLOR.g, GHOST_HOUSE_GATE_COLOR.b, GHOST_HOUSE_GATE_COLOR.a);
    SDL_RenderFillRect(renderer, &GHOST_HOUSE_GATE_RECT);
}

void Game::renderRemainingLives() {
    for (int i = 0; i < remainingLives; i++) {
        SDL_Rect rect = {
            GameConst::TILE_SIZE / 2 + i * GameConst::ENTITY_TEXTURE_SIZE,
            GameConst::WINDOW_HEIGHT - GameConst::ENTITY_TEXTURE_SIZE,
            GameConst::ENTITY_TEXTURE_SIZE,
            GameConst::ENTITY_TEXTURE_SIZE
        };
        SDL_RenderCopy(renderer, pacman->getTextureOriented(), nullptr, &rect);
    }
}

void Game::renderText() {
    renderText("LEVEL " + std::to_string(gameLevel), 0, 0, TEXT_COLOR);
    renderText("SCORE " + std::to_string(gameScore), 0, 3 * GameConst::TILE_SIZE / 2, TEXT_COLOR);
}

void Game::renderMap() {
    tilingManager->renderTiles();
    renderGhostHouseGate();
    pelletManager->renderPellets();
    renderRemainingLives();
    renderText();
}
