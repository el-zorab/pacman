#pragma once

#include <array>
#include <memory>

#include "gameConst.hpp"
#include "timer.hpp"

class PelletManager {

public:
    PelletManager();

    void removePellet(int x, int y);
    void renderPellets();

private:
    enum class PelletType : int {
        NONE = 0,
        PELLET = 1,
        ENERGIZER = 2
    };

    const int PELLET_SIZE = 4;
    const int ENERGIZER_SIZE = GameConst::TILE_SIZE * 3 / 4;

    std::array<std::array<PelletType, GameConst::TILE_ROWS>, GameConst::TILE_COLS> pellets;

    std::unique_ptr<Timer> energizerBlinkTimer;
    const Uint32 ENERGIZER_BLINK_INTERVAL_MS = 500;
    bool showEnergizers;
};
