#include <fstream>
#include <SDL2/SDL.h>
#include <string>

#include "game.hpp"
#include "orientation.hpp"
#include "tilingManager.hpp"

const std::string TILING_FILE_PATH = "res/tiling.dat";

TilingManager::TilingManager() {
    std::ifstream tilingFile(TILING_FILE_PATH);

    for (int j = 0; j < GameConst::TILE_ROWS; j++) {
        for (int i = 0; i < GameConst::TILE_COLS; i++) {
            char c;
            tilingFile >> c;
            tiling[i][j] = int(c) - '0';
        }
    }

    tilingFile.close();
}


TileState TilingManager::getTileState(int x, int y) {
    if (x < 0 || y < 0 || x >= GameConst::TILE_COLS || y >= GameConst::TILE_ROWS) {
        return y == GameConst::WARPING_TILE_ROW ? TileState::FREE : TileState::SOLID;
    }
    return tiling[x][y] ? TileState::SOLID : TileState::FREE;
}

void TilingManager::renderTiles() {
    SDL_Renderer *renderer = Game::getInstance().getRenderer();

    SDL_SetRenderDrawColor(renderer, 52, 110, 235, 255);
    for (int i = 0; i < GameConst::TILE_COLS; i++) {
        for (int j = 0; j < GameConst::TILE_ROWS; j++) {
            if (getTileState(i, j) == TileState::FREE) {
                continue;
            }

            int neighbours = 0;
            for (int k = 0; k < ORIENTATIONS; k++) {
                Orientation orientation = static_cast<Orientation>(k);
                Entity2D vec = orientationToVector(orientation);
                neighbours |= getTileState(i + vec.x, j + vec.y)
            }

            SDL_Rect tileRect = {
                GameConst::TILE_SIZE * i,
                GameConst::TILE_SIZE * j,
                GameConst::TILE_SIZE,
                GameConst::TILE_SIZE
            };

            SDL_RenderFillRect(renderer, &tileRect);
        }
    }
}
