#pragma once
#include <SDL.h>
#include <cmath>
#include "math.hpp"
namespace utils {
    inline float hireTimeInSeconds() {
        float t = SDL_GetTicks();
        t *= 0.001f;
        return t;
    }
}

namespace utills {
    Vector2f calculateDirection(const Vector2f& from, const Vector2f& to);
   inline float calculateDistance(const Vector2f &pos1, const Vector2f &pos2) {
    float dx = pos2.x - pos1.x;
    float dy = pos2.y - pos1.y;
    return sqrt(dx * dx + dy * dy);
   };

}