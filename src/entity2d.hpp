#pragma once

struct Entity2D {
    int x, y;
};

Entity2D operator+(Entity2D a, Entity2D b);
Entity2D operator-(Entity2D a, Entity2D b);
Entity2D operator*(Entity2D a, int scalar);
Entity2D operator/(Entity2D a, int scalar);
