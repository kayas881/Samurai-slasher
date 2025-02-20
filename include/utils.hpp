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
}