#pragma once

#include "ghost.hpp"

class Inky : public Ghost {

public:
    Inky();

private:
    Entity2D getTargetTile() override;
    SDL_Color getTextureColor() override;
    std::string getTexturePath() override;
    int getVelocity() override;

};
