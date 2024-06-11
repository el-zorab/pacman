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
}

Entity2D Blinky::getChaseTargetTile() {
    Entity2D pacmanCurrTile = Game::getInstance().getPacman().getCurrentTile();
    return pacmanCurrTile;
}

Entity2D Blinky::getScatterTargetTile() {
    return { 25, 0 };
}

SDL_Color Blinky::getTextureColor() {
    return { 255, 0, 0, 255 };
};
