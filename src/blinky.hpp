#pragma once

#include "ghost.hpp"

class Blinky : public Ghost {

public:
    Blinky();

private:
    Entity2D getTargetTile() override;
    SDL_Color getTextureColor() override;
    std::string getTexturePath() override;
    int getVelocity() override;

};
