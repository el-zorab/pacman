#include "blinky.hpp"
#include "gameConst.hpp"

using GameConst::TILE_SIZE;

Blinky::Blinky(SDL_Renderer *renderer) {
    this->renderer = renderer;

    posTile.x = 13;
    posTile.y = 11; 
    pos.x = posTile.x * TILE_SIZE;
    pos.y = posTile.y * TILE_SIZE;
    orientation = Orientation::RIGHT;
}

void Blinky::update(Entity2D pacmanPosTile) {
    targetTile = pacmanPosTile;
}

void Blinky::render() {
    SDL_Rect blinkyRect = { targetTile.x * TILE_SIZE, targetTile.y * TILE_SIZE, TILE_SIZE, TILE_SIZE };
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &blinkyRect);

}
