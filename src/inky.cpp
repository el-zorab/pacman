#include "game.hpp"
#include "gameConst.hpp"
#include "inky.hpp"

Inky::Inky() {}

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
