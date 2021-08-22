#ifndef GLOBAL_H
#define GLOBAL_H

#include <cmath>
#include <limits>
#include <memory>

// Usings

using std::shared_ptr;
using std::make_shared;
using std::sqrt;

// Constants

const float infinity = std::numeric_limits<float>::infinity();
const float pi = 3.1415926535897932385;

// Utility Functions

inline float degreesToRadians(float degrees)
{
    return degrees * pi / 180.0;
}

// Common Headers

#include "Ray.hpp"
#include "Vector.hpp"

#endif