#ifndef GLOBAL_H
#define GLOBAL_H

#include <cmath>
#include <vector>
#include <limits>
#include <memory>
#include <cstdlib>

// Class Declarations

class Vector3f;
class Ray;
class RGBSpectrum;
typedef RGBSpectrum Spectrum;
class Object;
class Light;

// Usings

using std::make_shared;
using std::shared_ptr;
using std::sqrt;

// Constants

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;
const double invPi = 1 / pi;

// Utility Functions

inline double degrees_to_radians(double degrees)
{
    return degrees * pi / 180.0;
}

inline double random_double()
{
    // Returns a random real in [0,1).
    return rand() / (RAND_MAX + 1.0);
}

inline double random_double(double min, double max)
{
    // Returns a random real in [min,max).
    return min + (max - min) * random_double();
}

inline double clamp(double x, double min, double max)
{
    if (x < min)
        return min;
    if (x > max)
        return max;
    return x;
}

inline int random_int(int min, int max)
{
    // Returns a random integer in [min, max]
    return static_cast<int>(random_double(min, max + 1));
}

inline double safe_sqrt(const double &a)
{
    double tmp = a;
    if (a < 0.0f) return 0;
    else return std::sqrt(tmp);
}

// Common Headers

#include "ray.h"
#include "vector.h"

#endif