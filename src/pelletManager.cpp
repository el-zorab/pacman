#include <fstream>
#include <SDL2/SDL.h>
#include <string>

#include "game.hpp"
#include "pelletManager.hpp"

const std::string PELLET_FILE_PATH = "res/pellet.dat";

const SDL_Color PELLET_COLOR = { 237, 180, 180, 255 };

PelletManager::PelletManager() {
    std::ifstream pelletFile(PELLET_FILE_PATH);

    for (int j = 0; j < GameConst::TILE_ROWS; j++) {
        for (int i = 0; i < GameConst::TILE_COLS; i++) {
            char c;
            pelletFile >> c;
            if (c == '1') {
                pellets[i][j] = PelletType::PELLET;
            } else {
                pellets[i][j] = PelletType::NONE;
            }
        }
    }

    pelletFile.close();
}

void PelletManager::renderPellets() {
    SDL_Renderer *renderer = Game::getInstance().getRenderer();

    SDL_SetRenderDrawColor(renderer, PELLET_COLOR.r, PELLET_COLOR.g, PELLET_COLOR.b, PELLET_COLOR.a);

    for (int i = 0; i < GameConst::TILE_COLS; i++) {
        for (int j = 0; j < GameConst::TILE_ROWS; j++) {
            if (pellets[i][j] == PelletType::PELLET) {
                SDL_Rect pelletRect = {
                    i * GameConst::TILE_SIZE + GameConst::TILE_SIZE / 2 - PELLET_SIZE / 2,
                    j * GameConst::TILE_SIZE + GameConst::TILE_SIZE / 2 - PELLET_SIZE / 2,
                    PELLET_SIZE,
                    PELLET_SIZE
                };

                SDL_RenderFillRect(renderer, &pelletRect);
            }
        }
    }
}

void PelletManager::removePellet(int x, int y) {
    if (x >= 0 && y >= 3 && x < GameConst::TILE_COLS && y < GameConst::TILE_ROWS - 2) {
        pellets[x][y] = PelletType::NONE;
    }
}
