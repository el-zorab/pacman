#pragma once

namespace GameConstants {
    const int TILE_ROWS = 31;
    const int TILE_COLS = 28;
    const int TILE_SIZE = 16;

    const int BORDER_SIZE = 32;

    const int WINDOW_WIDTH = TILE_SIZE * TILE_COLS;
    const int WINDOW_HEIGHT = TILE_SIZE * TILE_ROWS;

    const int WINDOW_BORDERED_WIDTH = WINDOW_WIDTH + 2 * BORDER_SIZE;
    const int WINDOW_BORDERED_HEIGHT = WINDOW_HEIGHT + 2 * BORDER_SIZE;
};
