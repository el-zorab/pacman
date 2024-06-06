#include "clyde.hpp"
#include "game.hpp"
#include "gameConst.hpp"

Clyde::Clyde() {}

Entity2D Clyde::getTargetTile() {
    Entity2D pacmanCurrTile = Game::getInstance().getPacman().getCurrentTile();
    Entity2D distVector = pacmanCurrTile - getCurrentTile();
    int distance = distVector.x * distVector.x + distVector.y * distVector.y;
    if (distance >= 64) {
        return pacmanCurrTile;
    } else {
        return { 0, 34 };
    }
}

SDL_Color Clyde::getTextureColor() {
    return { 255, 127, 0, 255 };
};

