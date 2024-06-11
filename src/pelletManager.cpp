#include <fstream>
#include <SDL2/SDL.h>
#include <string>

#include "game.hpp"
#include "pelletManager.hpp"

const std::string PELLET_FILE_PATH = "res/pellets.dat";

const SDL_Color PELLET_COLOR = { 255, 255, 255, 255 };

PelletManager::PelletManager() {
    totalPellets = 0;
    std::ifstream pelletFile(PELLET_FILE_PATH);

    for (int j = 0; j < GameConst::TILE_ROWS; j++) {
        for (int i = 0; i < GameConst::TILE_COLS; i++) {
            char c;
            pelletFile >> c;
            pellets[i][j] = static_cast<PelletType>((int) (c - '0'));
            if (pellets[i][j] != PelletType::NONE) totalPellets++;
        }
    }

    pelletFile.close();

    remainingPellets = totalPellets;

    energizerBlinkTimer = std::make_unique<Timer>();
    energizerBlinkTimer->start();
    showEnergizers = true;
}

int PelletManager::getRemainingPellets() {
    return remainingPellets;
}

int PelletManager::getEatenPellets() {
    return totalPellets - remainingPellets;
}

void PelletManager::renderPellets() {
    if (energizerBlinkTimer->getTicks() >= ENERGIZER_BLINK_INTERVAL_MS) {
        showEnergizers = !showEnergizers;
        energizerBlinkTimer->start();
    }

    SDL_Renderer *renderer = Game::getInstance().getRenderer();

    SDL_SetRenderDrawColor(renderer, PELLET_COLOR.r, PELLET_COLOR.g, PELLET_COLOR.b, PELLET_COLOR.a);

    for (int i = 0; i < GameConst::TILE_COLS; i++) {
        for (int j = 0; j < GameConst::TILE_ROWS; j++) {
            SDL_Rect pelletRect;

            if (pellets[i][j] == PelletType::PELLET) {
                pelletRect = {
                    i * GameConst::TILE_SIZE + GameConst::TILE_SIZE / 2 - PELLET_SIZE / 2,
                    j * GameConst::TILE_SIZE + GameConst::TILE_SIZE / 2 - PELLET_SIZE / 2,
                    PELLET_SIZE,
                    PELLET_SIZE
                };
            } else if (pellets[i][j] == PelletType::ENERGIZER && showEnergizers) {
                pelletRect = {
                    i * GameConst::TILE_SIZE + GameConst::TILE_SIZE / 2 - ENERGIZER_SIZE / 2,
                    j * GameConst::TILE_SIZE + GameConst::TILE_SIZE / 2 - ENERGIZER_SIZE / 2,
                    ENERGIZER_SIZE,
                    ENERGIZER_SIZE
                };
            }

            SDL_RenderFillRect(renderer, &pelletRect);
        }
    }
}

void PelletManager::removePellet(int x, int y) {
    if (x >= 0 && y >= 3 && x < GameConst::TILE_COLS && y < GameConst::TILE_ROWS - 2) {
        if (pellets[x][y] != PelletType::NONE) {
            pellets[x][y] = PelletType::NONE;
            remainingPellets--;
        }
    }
}
