#include <fstream>
#include <string>

#include "tilingManager.hpp"

const std::string TILING_FILE_PATH = "res/tiling.dat";

TilingManager::TilingManager(){}

void TilingManager::loadTiling() {
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
