#pragma once

#include <array>
#include <memory>

#include "gameConstants.hpp"

class TilingManager {

public:
    static TilingManager &getInstance();

    void loadTiling();
    int getTilingAt(int x, int y);

private:
    TilingManager();

    std::array<std::array<bool, GameConstants::TILE_ROWS>, GameConstants::TILE_COLS> tiling;

};
