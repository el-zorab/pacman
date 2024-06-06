#pragma once

#include "ghost.hpp"

class Pinky : public Ghost {

public:
    Pinky();

private:
    Entity2D getTargetTile() override;
    SDL_Color getTextureColor() override;
};
