#pragma once

namespace GameConstants {
    const int TILE_ROWS = 30;
    const int TILE_COLS = 27;
    const int TILE_SIZE = 16;

    const int BORDER_SIZE = TILE_SIZE;

    const int WINDOW_WIDTH = TILE_SIZE * TILE_COLS;
    const int WINDOW_HEIGHT = TILE_SIZE * TILE_ROWS;

    const int WINDOW_BORDERED_WIDTH = WINDOW_WIDTH + 2 * BORDER_SIZE;
    const int WINDOW_BORDERED_HEIGHT = WINDOW_HEIGHT + 2 * BORDER_SIZE;
};
