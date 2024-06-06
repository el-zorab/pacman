#include "blinky.hpp"
#include "game.hpp"
#include "gameConst.hpp"

Blinky::Blinky() {}

Entity2D Blinky::getTargetTile() {
    Entity2D pacmanCurrTile = Game::getInstance().getPacman().getCurrentTile();
    return pacmanCurrTile;
}

SDL_Color Blinky::getTextureColor() {
    return { 255, 0, 0, 255 };
};
