#pragma once

namespace GameConst {
    static const int TILE_ROWS = 31;
    static const int TILE_COLS = 28;
    static const int TILE_SIZE = 16;

    static const int BORDER_SIZE = 32;

    static const int WINDOW_WIDTH  = TILE_SIZE * TILE_COLS;
    static const int WINDOW_HEIGHT = TILE_SIZE * TILE_ROWS;

    static const int WINDOW_BORDERED_WIDTH  = WINDOW_WIDTH  + 2 * BORDER_SIZE;
    static const int WINDOW_BORDERED_HEIGHT = WINDOW_HEIGHT + 2 * BORDER_SIZE;
}
