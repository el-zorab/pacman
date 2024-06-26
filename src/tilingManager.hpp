#pragma once

#include <array>
#include <SDL2/SDL.h>

#include "gameConst.hpp"

enum class TileState {
    FREE, SOLID
};

class TilingManager {

public:
    TilingManager();

    TileState getTileState(int x, int y);
    void renderTiles();

private:
    std::array<std::array<bool, GameConst::TILE_ROWS>, GameConst::TILE_COLS> solidTile;

    SDL_Texture *tilingTexture;

};
