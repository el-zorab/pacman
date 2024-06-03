#pragma once

#include <array>

#include "gameConst.hpp"

enum class TileState {
    FREE, SOLID
};

class TilingManager {

public:
    TilingManager();

    void loadTiling();
    TileState getTileState(int x, int y);

private:
    std::array<std::array<bool, GameConst::TILE_ROWS>, GameConst::TILE_COLS> tiling;

};
