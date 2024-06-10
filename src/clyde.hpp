#pragma once

#include "ghost.hpp"

class Clyde : public Ghost {

public:
    Clyde();

private:
    Entity2D getChaseTargetTile() override;
    Entity2D getScatterTargetTile() override;
    SDL_Color getTextureColor() override;

};
