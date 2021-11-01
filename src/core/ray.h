#ifndef RAY_H
#define RAY_H

#include "vector.h"

class Ray
{
public:
    Ray() {}
    Ray(const Point3f &origin, const Vector3f &direction, double time = 0.0)
        : orig(origin), dir(direction), tm(time)
    {}

    Point3f origin() const  { return orig; }
    Vector3f direction() const { return dir; }
    double time() const    { return tm; }

    Point3f at(double t) const
    {
        return orig + t * dir;
    }

public:
    Point3f orig;
    Vector3f dir;
    double tm;
};

#endif