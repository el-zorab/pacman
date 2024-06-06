#include "game.hpp"
#include "gameConst.hpp"
#include "inky.hpp"

Inky::Inky() {}

Entity2D Inky::getTargetTile() {
    Entity2D pacmanOrientationVector = orientationToVector(Game::getInstance().getPacman().getOrientation());
    Entity2D pacmanCurrTile = Game::getInstance().getPacman().getCurrentTile();
    Entity2D blinkyCurrentTile = Game::getInstance().getBlinky().getCurrentTile();
    Entity2D inkyTargetTile = (pacmanCurrTile + pacmanOrientationVector * 2) * 2 - blinkyCurrentTile;
    return inkyTargetTile;
}

SDL_Color Inky::getTextureColor() {
    return { 0, 255, 255, 255 };
};
