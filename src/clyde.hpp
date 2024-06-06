#pragma once

#include "ghost.hpp"

class Clyde : public Ghost {

public:
    Clyde();

private:
    Entity2D getTargetTile() override;
    SDL_Color getTextureColor() override;

};
