#ifndef RAY_H
#define RAY_H

#include "global.h"
#include "vector.h"

class Ray
{
public:
    Ray() : tMax(Infinity), time(0.f) {}
    Ray(const Vector3f &o, const Vector3f &d, float tMax = Infinity,
        float time = 0.f)
        : origin(o), direction(d), tMax(tMax), time(time) {}

    Vector3f operator () (float t) const { return origin + direction * t; }

public:
    Vector3f origin;
    Vector3f direction;
    mutable float tMax;
    float time;
};

class RayDifferential : public Ray
{
public:
    RayDifferential() { hasDifferentials = false; }
    RayDifferential(const Vector3f &o, const Vector3f &d, float tMax = Infinity,
                    float time = 0.f)
                    : Ray(o, d, tMax, time)
                    {
                        hasDifferentials = false;
                    }
    RayDifferential(const Ray &ray) : Ray(ray)
    {
        hasDifferentials = false;
    }

    void ScaleDifferentials(float s)
    {
        rxOrigin = origin + (rxOrigin - origin) * s;
        ryOrigin = origin + (ryOrigin - origin) * s;
        rxDirection = direction + (rxDirection - direction) * s;
        ryDirection = direction + (ryDirection - direction) * s;
    }

public:
    bool hasDifferentials;
    Vector3f rxOrigin, ryOrigin;
    Vector3f rxDirection, ryDirection;
};

#endif