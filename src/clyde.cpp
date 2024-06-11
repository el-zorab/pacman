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
    mode = Mode::IN_HOUSE;
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

Entity2D Clyde::getScatterTargetTile() {
    return { 0, 34 };
}

SDL_Color Clyde::getTextureColor() {
    return { 255, 127, 0, 255 };
};

