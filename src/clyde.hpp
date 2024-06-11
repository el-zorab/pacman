#pragma once

#include "ghost.hpp"

class Clyde : public Ghost {

public:
    Clyde();

protected:
    void initChild() override;

private:
    Entity2D getChaseTargetTile() override;
    Entity2D getScatterTargetTile() override;
    SDL_Color getTextureColor() override;

};
