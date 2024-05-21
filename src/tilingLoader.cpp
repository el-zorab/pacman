#include <fstream>
#include <string>

#include "tilingLoader.hpp"

std::string TILING_FILE_PATH = "res/tiling.dat";

void TilingLoader::loadTiling() {
    std::ifstream tilingFile(TILING_FILE_PATH);

    for (int i = 0; i < GameConstants::TILE_ROWS; i++) {
        for (int j = 0; j < GameConstants::TILE_COLS; j++) {
            char c;
            tilingFile >> c;
            tiling[i][j] = int(c) - '0';
        }
    }
}

auto TilingLoader::getTiling() {
    return tiling;
}
