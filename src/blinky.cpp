#include "blinky.hpp"
#include "game.hpp"
#include "gameConst.hpp"

Blinky::Blinky() {}

Entity2D Blinky::getTargetTile() {
    Entity2D pacmanTargetTile = Game::getInstance().getPacman().getTargetTile();
    return pacmanTargetTile;
}

SDL_Color Blinky::getTextureColor() {
    return { 255, 0, 0, 255 };
};

std::string Blinky::getTexturePath() {
    return "arrow.png";
};

int Blinky::getVelocity() {
    return 75 * GameConst::VELOCITY_ONE_TILE_PER_SEC / 10;
}
