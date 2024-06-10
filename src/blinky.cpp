#include "blinky.hpp"
#include "game.hpp"
#include "gameConst.hpp"

Blinky::Blinky() {}

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
