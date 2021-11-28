#ifndef GLOBAL_H
#define GLOBAL_H

#include <cmath>
#include <vector>
#include <limits>
#include <memory>
#include <cstdlib>

// Class Declarations

class Transform;
class Ray;
class RGBSpectrum;
typedef RGBSpectrum Spectrum;
class Object;
class Light;
struct VisibilityTester;
class Sampler;
class Camera;
class Material;
class Film;
struct HitRecord;
class Scene;
class Renderer;
class Fresnel;
class BxDF;
class BSDF;
class Medium;
class MicrofacetDistribution;
class PhaseFunction;
struct MediumRecord;
enum class TransportMode;

// Usings

using std::make_shared;
using std::shared_ptr;
using std::sqrt;

// Constants

const float INF = std::numeric_limits<float>::infinity();
static constexpr float Infinity = std::numeric_limits<float>::infinity();
static constexpr float MaxFloat = std::numeric_limits<float>::max();
const double PI = 3.1415926535897932385;
const double invPI = 1 / PI;
const float Inv4Pi = 0.07957747154594766788;
const float PiOver2 = 1.57079632679489661923;
const float PiOver4 = 0.78539816339744830961;
const float ShadowEpsilon = 0.0001f;

// Utility Functions

template <typename T, typename U, typename V>
inline T Clamp(T val, U low, V high) {
    if (val < low) return low;
    else if (val > high) return high;
    else return val;
}

inline float Lerp(float t, float v1, float v2) {
    return (1 - t) * v1 + t * v2;
}

inline double degrees_to_radians(double degrees)
{
    return degrees * PI / 180.0;
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

inline float PowerHeuristic(int nf, float fPdf, int ng, float gPdf) {
    float f = nf * fPdf, g = ng * gPdf;
    return (f * f) / (f * f + g * g);
}

#endif