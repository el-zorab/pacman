#pragma once

#include <array>
#include <memory>
#include <SDL2/SDL.h>

#include "tile.hpp"
#include "timer.hpp"

class Pacman {

public:
    Pacman(SDL_Renderer *renderer);
    void close();
    
    enum class State { MOVING, WARPING, IDLE };

    enum Orientation {
        LEFT = 0,
        RIGHT = 1,
        UP = 2,
        DOWN = 3
    };

    void setDesiredOrientation(Orientation orientation);

    void update(float deltaTime);
    void render();

private:
    SDL_Renderer *renderer;
    SDL_Texture *textureOriented;
    SDL_Texture *textureUnoriented;

    static const int PACMAN_TEXTURE_W = 32;
    static const int PACMAN_TEXTURE_H = 32;
    static const int PACMAN_SPEED = 2;

    State state;
    Orientation orientation, desiredOrientation;
    int x, y, xTile, yTile, xEndTile, yEndTile;
    bool moving, warping;

    static const int NEIGHBOUR_TILES_COUNT = 2;
    std::array<std::array<Tile, NEIGHBOUR_TILES_COUNT>, 4> neighbourTiles;

    std::unique_ptr<Timer> animationTimer;
    int animationIndex;

    bool areTilesFree(Orientation orientation);
    int textureRotationFromOrientation(Orientation orientation);
    void updateMoving();
    void updateWarping();
};
