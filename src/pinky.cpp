#include "game.hpp"
#include "gameConst.hpp"
#include "pinky.hpp"

Pinky::Pinky() {}

Entity2D Pinky::getChaseTargetTile() {
    Entity2D pacmanOrientationVector = orientationToVector(Game::getInstance().getPacman().getOrientation());
    Entity2D pacmanCurrTile = Game::getInstance().getPacman().getCurrentTile();
    Entity2D pinkyTargetTile = pacmanCurrTile + pacmanOrientationVector * 4;
    return pinkyTargetTile;
}

Entity2D Pinky::getScatterTargetTile() {
    return { 2, 0 };
}

SDL_Color Pinky::getTextureColor() {
    return { 255, 0, 255, 255 };
};
