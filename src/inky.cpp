#include "game.hpp"
#include "gameConst.hpp"
#include "inky.hpp"

using GameConst::UNITS_PER_TILE;

Inky::Inky() {}

void Inky::initChild() {
    currPos = {
        11 * UNITS_PER_TILE + UNITS_PER_TILE / 2,
        17 * UNITS_PER_TILE
    };
    orientation = Orientation::UP;

    state = State::WANDERING;

    scatterTargetTile = { 27, 34 };

    ghostColor = { 0, 255, 255, 255 };
}

Entity2D Inky::getChaseTargetTile() {
    Entity2D pacmanOrientationVector = orientationToVector(Game::getInstance().getPacman().getOrientation());
    Entity2D pacmanCurrTile = Game::getInstance().getPacman().getCurrentTile();
    Entity2D blinkyCurrentTile = Game::getInstance().getBlinky().getCurrentTile();
    Entity2D inkyTargetTile = (pacmanCurrTile + pacmanOrientationVector * 2) * 2 - blinkyCurrentTile;
    return inkyTargetTile;
}
