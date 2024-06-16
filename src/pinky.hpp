#pragma once

#include "ghost.hpp"

class Pinky : public Ghost {

public:
    Pinky();

protected:
    void initChild() override;

private:
    Entity2D getChaseTargetTile() override;

};
