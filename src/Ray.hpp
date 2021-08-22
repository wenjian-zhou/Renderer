#ifndef RAY_H
#define RAY_H

#include "Vector.hpp"

class Ray
{
public:
    Ray() {}
    Ray(const Point3 &origin, const Vector3f &direction)
        : orig(origin), dir(direction)
    {}

    Point3 origin() const { return orig; }
    Vector3f direction() const { return dir; }

    Point3 at(float t) const
    {
        return orig + t * dir;
    }

public:
    Point3 orig;
    Vector3f dir;
};

#endif