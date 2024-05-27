#include "orientation.hpp"

int orientationRotationDeg(Orientation orientation) {
    switch (orientation) {
        case Orientation::RIGHT: return 0;   break;
        case Orientation::DOWN:  return 90;  break;
        case Orientation::LEFT:  return 180; break;
        case Orientation::UP:    return 270; break;
    }
    return 0;
}

Entity2D orientationVec(Orientation orientation) {
    switch (orientation) {
        case Orientation::RIGHT: return Entity2D {  1,  0 }; break;
        case Orientation::DOWN:  return Entity2D {  0,  1 }; break;
        case Orientation::LEFT:  return Entity2D { -1,  0 }; break;
        case Orientation::UP:    return Entity2D {  0, -1 }; break;
    }
    return Entity2D { 0, 0 };
}
