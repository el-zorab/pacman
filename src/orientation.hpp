#pragma once

#include "entity2d.hpp"

enum class Orientation {
    RIGHT, DOWN, LEFT, UP
};

int orientationRotationDeg(Orientation orientation);
Entity2D orientationVec(Orientation orientation);
