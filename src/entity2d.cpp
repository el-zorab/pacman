#include "entity2d.hpp"

Entity2D operator+(Entity2D a, Entity2D b) {
    Entity2D c;
    c.x = a.x + b.x;
    c.y = a.y + b.y;
    return c;
}

Entity2D operator-(Entity2D a, Entity2D b) {
    Entity2D c;
    c.x = a.x - b.x;
    c.y = a.y - b.y;
    return c;
}

Entity2D operator*(Entity2D a, int scalar) {
    Entity2D b;
    b.x = a.x * scalar;
    b.y = a.y * scalar;
    return b;
}

Entity2D operator/(Entity2D a, int scalar) {
    Entity2D b;
    b.x = a.x / scalar;
    b.y = a.y / scalar;
    return b;
}

bool operator==(Entity2D a, Entity2D b) {
    return a.x == b.x && a.y == b.y;
}
