#pragma once

namespace GameConst {

    static const int BORDER_SIZE = 32;
    static const int TILE_SIZE = 20;

    static const int TILE_ROWS = 36;
    static const int TILE_COLS = 28;

    static const int WINDOW_WIDTH  = TILE_SIZE * TILE_COLS;
    static const int WINDOW_HEIGHT = TILE_SIZE * TILE_ROWS;

    static const int WINDOW_BORDERED_WIDTH  = WINDOW_WIDTH  + 2 * BORDER_SIZE;
    static const int WINDOW_BORDERED_HEIGHT = WINDOW_HEIGHT + 2 * BORDER_SIZE;

    static const int UNITS_PER_PIXEL = 100000;
    static const int UNITS_PER_TILE = UNITS_PER_PIXEL * TILE_SIZE;

    static const int ENTITY_TEXTURE_SIZE = 2 * TILE_SIZE;

    static const int WARPING_TILE_ROW = 17;
    static const int WARPING_MIN_X = -ENTITY_TEXTURE_SIZE * UNITS_PER_PIXEL;
    static const int WARPING_MAX_X = (TILE_COLS - 1) * UNITS_PER_TILE + ENTITY_TEXTURE_SIZE * UNITS_PER_PIXEL;

    static const int VELOCITY_TILES_PER_SEC = UNITS_PER_TILE / 1000;
    
}
