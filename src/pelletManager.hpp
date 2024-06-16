#pragma once

#include <array>
#include <memory>

#include "gameConst.hpp"
#include "timer.hpp"

enum class PelletType : int {
    NONE = 0,
    PELLET = 1,
    ENERGIZER = 2
};

class PelletManager {

public:
    PelletManager();

    int getEatenPellets();
    int getRemainingPellets();
    PelletType removePellet(int x, int y);
    void renderPellets();

private:
    const int PELLET_SIZE = 6;
    const int ENERGIZER_SIZE = GameConst::TILE_SIZE * 3 / 4;

    int remainingPellets;
    int totalPellets;

    std::array<std::array<PelletType, GameConst::TILE_ROWS>, GameConst::TILE_COLS> pellets;

    std::unique_ptr<Timer> energizerBlinkTimer;
    const int ENERGIZER_BLINK_INTERVAL_MS = 500;
    bool showEnergizers;
};
