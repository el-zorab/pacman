#include <fstream>
#include <string>

#include "tilingManager.hpp"

std::string TILING_FILE_PATH = "res/tiling.dat";

TilingManager::TilingManager() {}

TilingManager &TilingManager::getInstance() {
    static std::unique_ptr<TilingManager> instance(new TilingManager());
    return *instance;
}

void TilingManager::loadTiling() {
    std::ifstream tilingFile(TILING_FILE_PATH);

    for (int j = 0; j < GameConstants::TILE_ROWS; j++) {
        for (int i = 0; i < GameConstants::TILE_COLS; i++) {
            char c;
            tilingFile >> c;
            tiling[i][j] = int(c) - '0';
        }
    }

    tilingFile.close();
}

int TilingManager::getTilingAt(int x, int y) {
    if (x < 0 || y < 0 || x >= GameConstants::TILE_COLS || y >= GameConstants::TILE_ROWS) return 0;
    return tiling[x][y];
}
