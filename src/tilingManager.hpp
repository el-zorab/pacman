#pragma once

#include <array>

#include "gameConstants.hpp"

class TilingManager {

public:
    TilingManager();

    void loadTiling();
    int isTileFree(int x, int y);

private:
    std::array<std::array<bool, GameConstants::TILE_ROWS>, GameConstants::TILE_COLS> tiling;

};
