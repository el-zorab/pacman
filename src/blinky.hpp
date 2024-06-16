#pragma once

#include "ghost.hpp"

class Blinky : public Ghost {

public:
    Blinky();

protected:
    void initChild() override;

private:
    Entity2D getChaseTargetTile() override;

};
