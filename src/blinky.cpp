#include "blinky.hpp"
#include "game.hpp"
#include "gameConst.hpp"

using GameConst::UNITS_PER_TILE;

Blinky::Blinky() {}

void Blinky::initChild() {
    currPos = {
        13 * UNITS_PER_TILE + UNITS_PER_TILE / 2,
        14 * UNITS_PER_TILE
    };
    orientation = Orientation::RIGHT;

    state = State::NORMAL_OPERATION;

    scatterTargetTile = { 25, 0 };

    ghostColor = { 255, 31, 31, 255 };
}

Entity2D Blinky::getChaseTargetTile() {
    Entity2D pacmanCurrTile = Game::getInstance().getPacman().getCurrentTile();
    return pacmanCurrTile;
}
