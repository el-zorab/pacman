#include <fstream>
#include <string>

#include "game.hpp"
#include "orientation.hpp"
#include "tilingManager.hpp"

using GameConst::TILE_SIZE;

const std::string TILING_FILE_PATH = "res/tiling.dat";

TilingManager::TilingManager() {
    std::ifstream tilingFile(TILING_FILE_PATH);

    for (int j = 0; j < GameConst::TILE_ROWS; j++) {
        for (int i = 0; i < GameConst::TILE_COLS; i++) {
            char c;
            tilingFile >> c;
            tiling[i][j] = int(c) - '0';
        }
    }

    tilingFile.close();

    tilingTexture = Game::getInstance().getTextureManager().loadTexture("tiles.png");
    SDL_SetTextureColorMod(tilingTexture, 52, 110, 235);
}


TileState TilingManager::getTileState(int x, int y) {
    if (x < 0 || y < 0 || x >= GameConst::TILE_COLS || y >= GameConst::TILE_ROWS) {
        return y == GameConst::WARPING_TILE_ROW ? TileState::FREE : TileState::SOLID;
    }
    return tiling[x][y] ? TileState::SOLID : TileState::FREE;
}

void TilingManager::renderTiles() {
    SDL_Renderer *renderer = Game::getInstance().getRenderer();

    for (int i = 0; i < GameConst::TILE_COLS; i++) {
        for (int j = 0; j < GameConst::TILE_ROWS; j++) {
            if (getTileState(i, j) == TileState::FREE) {
                continue;
            }

            SDL_Rect destRect = {
                TILE_SIZE * i,
                TILE_SIZE * j,
                TILE_SIZE,
                TILE_SIZE
            };

            int neighbours = 0;
            for (int k = 0; k < ORIENTATIONS; k++) {
                Orientation orientation = static_cast<Orientation>(k);
                Entity2D vec = orientationToVector(orientation);
                Entity2D neighbourTile = {
                    i + vec.x,
                    j + vec.y
                };
                if (neighbourTile.x < 0 || neighbourTile.x >= GameConst::TILE_COLS) continue;
                neighbours |= (getTileState(neighbourTile.x, neighbourTile.y) == TileState::SOLID) << k;
            }

            const int TEXTURE_TILE_SIZE = 32;

            SDL_Rect srcRect = { TEXTURE_TILE_SIZE * neighbours, 0, TEXTURE_TILE_SIZE, TEXTURE_TILE_SIZE };

            SDL_RenderCopy(renderer, tilingTexture, &srcRect, &destRect);
        }
    }
}
