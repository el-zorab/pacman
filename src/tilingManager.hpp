#pragma once

#include <array>

#include "game.hpp"

class TilingManager {

public:
    TilingManager();

    void loadTiling();
    int isTileFree(int x, int y);

private:
    std::array<std::array<bool, Game::TILE_ROWS>, Game::TILE_COLS> tiling;

};
