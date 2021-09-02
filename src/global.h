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

const float Infinity = std::numeric_limits<float>::infinity();
const float pi = 3.1415926535897932385;

// Utility Functions

inline float degreesToRadians(float degrees)
{
    return degrees * pi / 180.0;
}

<<<<<<< HEAD:src/global.h
inline float randomFloat()
{
    // Returns a random real in [0, 1).
    return rand() / (RAND_MAX + 1.0);
}

inline float randomFloat(float min, float max)
{
    // Returns a random real in [min, max)
    return min + (max - min) * randomFloat();
}

inline float clamp(float x, float min, float max)
{
    if (x < min) return min;
    if (x > max) return max;
    return x;
}

<<<<<<< HEAD:src/global.h
inline int random_int(int min, int max) {
    // Returns a random integer in [min,max].
    return static_cast<int>(randomFloat(min, max+1));
}
=======
=======
>>>>>>> parent of 163299f (add BVH):src/global.hpp
// Common Headers
>>>>>>> parent of 70be2a2 (update camera and material):src/global.hpp

// Common Headers

#endif