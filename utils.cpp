#include "include/utils.hpp"


namespace utills {
    Vector2f calculateDirection(const Vector2f& from, const Vector2f& to) {
        Vector2f direction = { to.x - from.x, to.y - from.y };
        float length = sqrt(direction.x * direction.x + direction.y * direction.y);
        if (length != 0) {
            direction.x /= length;
            direction.y /= length;
        }
        return direction;
    }
}