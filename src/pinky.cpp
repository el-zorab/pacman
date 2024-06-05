#include "game.hpp"
#include "gameConst.hpp"
#include "pinky.hpp"

Pinky::Pinky() {}

Entity2D Pinky::getTargetTile() {
    Entity2D pacmanOrientationVector = orientationToVector(Game::getInstance().getPacman().getOrientation());
    Entity2D pacmanTargetTile = Game::getInstance().getPacman().getTargetTile();
    Entity2D pinkyTargetTile = pacmanTargetTile + pacmanOrientationVector * 4;
    return pinkyTargetTile;
}

SDL_Color Pinky::getTextureColor() {
    return { 255, 0, 255, 255 };
};

std::string Pinky::getTexturePath() {
    return "arrow.png";
};

int Pinky::getVelocity() {
    return 75 * GameConst::VELOCITY_ONE_TILE_PER_SEC / 10;
}
