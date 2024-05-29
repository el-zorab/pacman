#pragma once

#include <array>

#include "gameConst.hpp"

class TilingManager {

public:
    TilingManager();

    void loadTiling();
    int isTileFree(int x, int y);

private:
    std::array<std::array<bool, GameConst::TILE_ROWS>, GameConst::TILE_COLS> tiling;

};
