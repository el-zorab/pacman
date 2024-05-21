#pragma once

#include <array>

#include "gameConstants.hpp"

class TilingLoader {

public:
    void loadTiling();
    auto getTiling();

private:
    std::array<std::array<bool, GameConstants::TILE_COLS>, GameConstants::TILE_ROWS> tiling;
};
