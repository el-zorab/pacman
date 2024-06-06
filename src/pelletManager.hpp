#pragma once

#include <array>

#include "gameConst.hpp"

class PelletManager {

public:
    PelletManager();

    void removePellet(int x, int y);
    void renderPellets();

private:
    enum class PelletType {
        NONE, PELLET, ENERGIZER
    };

    const int PELLET_SIZE = 4;

    std::array<std::array<PelletType, GameConst::TILE_ROWS>, GameConst::TILE_COLS> pellets;

};
