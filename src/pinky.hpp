#pragma once

#include "ghost.hpp"

class Pinky : public Ghost {

public:
    Pinky();

private:
    Entity2D getInitPos() override;
    Entity2D getChaseTargetTile() override;
    Entity2D getScatterTargetTile() override;
    SDL_Color getTextureColor() override;
};
