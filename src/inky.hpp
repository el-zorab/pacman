#pragma once

#include "ghost.hpp"

class Inky : public Ghost {

public:
    Inky();

protected:
    void initChild() override;

private:
    Entity2D getChaseTargetTile() override;

};
