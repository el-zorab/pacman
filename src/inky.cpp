#include "game.hpp"
#include "gameConst.hpp"
#include "inky.hpp"

Inky::Inky() {}

Entity2D Inky::getTargetTile() {
    Entity2D pacmanOrientationVector = orientationToVector(Game::getInstance().getPacman().getOrientation());
    Entity2D pacmanTargetTile = Game::getInstance().getPacman().getTargetTile();
    Entity2D blinkyCurrentTile = Game::getInstance().getBlinky().getCurrentTile();
    Entity2D inkyTargetTile = (pacmanTargetTile + pacmanOrientationVector * 2) * 2 - blinkyCurrentTile;
    return inkyTargetTile;
}

SDL_Color Inky::getTextureColor() {
    return { 0, 255, 255, 255 };
};

std::string Inky::getTexturePath() {
    return "arrow.png";
};

int Inky::getVelocity() {
    return 75 * GameConst::VELOCITY_ONE_TILE_PER_SEC / 10;
}
