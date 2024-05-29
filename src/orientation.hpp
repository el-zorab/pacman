#pragma once

#include <string>

#include "entity2d.hpp"

enum class Orientation : int {
    UP = 0,
    LEFT = 1,
    DOWN = 2,
    RIGHT = 3
};

const int ORIENTATIONS = 4;

bool orientationAreOpposites(Orientation x, Orientation y);
std::string orientationName(Orientation orientation);
int orientationToDeg(Orientation orientation);
Entity2D orientationToVector(Orientation orientation);
