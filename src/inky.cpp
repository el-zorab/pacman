#include "game.hpp"
#include "gameConst.hpp"
#include "inky.hpp"

using GameConst::UNITS_PER_TILE;

Inky::Inky() {}

Entity2D Inky::getInitPos() {
    return {
        15 * UNITS_PER_TILE + UNITS_PER_TILE / 2,
        17 * UNITS_PER_TILE
    };
}

Entity2D Inky::getChaseTargetTile() {
    Entity2D pacmanOrientationVector = orientationToVector(Game::getInstance().getPacman().getOrientation());
    Entity2D pacmanCurrTile = Game::getInstance().getPacman().getCurrentTile();
    Entity2D blinkyCurrentTile = Game::getInstance().getBlinky().getCurrentTile();
    Entity2D inkyTargetTile = (pacmanCurrTile + pacmanOrientationVector * 2) * 2 - blinkyCurrentTile;
    return inkyTargetTile;
}

Entity2D Inky::getScatterTargetTile() {
    return { 27, 34 };
}

SDL_Color Inky::getTextureColor() {
    return { 0, 255, 255, 255 };
};
