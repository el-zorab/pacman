#include "clyde.hpp"
#include "game.hpp"
#include "gameConst.hpp"

using GameConst::UNITS_PER_TILE;

Clyde::Clyde() {}

void Clyde::initChild() {
    currPos = {
        15 * UNITS_PER_TILE + UNITS_PER_TILE / 2,
        17 * UNITS_PER_TILE
    };
    orientation = Orientation::UP;

    state = State::WANDERING;

    scatterTargetTile = { 0, 34 };

    ghostColor = { 255, 127, 31, 255 };
}

Entity2D Clyde::getChaseTargetTile() {
    Entity2D pacmanCurrTile = Game::getInstance().getPacman().getCurrentTile();
    Entity2D distVector = pacmanCurrTile - getCurrentTile();
    int distance = distVector.x * distVector.x + distVector.y * distVector.y;
    if (distance >= 64) {
        return pacmanCurrTile;
    } else {
        return { 0, 34 };
    }
}
