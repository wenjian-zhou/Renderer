#ifndef RAY_H
#define RAY_H

#include "global.h"
#include "vector.h"

class Ray
{
public:
    Ray() : tMax(INF), time(0.f), medium(nullptr) {}
    Ray(const Point3f &o, const Vector3f &d, float tMax = INF, float time = 0.f, const Medium *medium = nullptr)
        : o(o), d(d), tMax(tMax), time(time), medium(medium) {}
    Point3f operator()(float t) const { return o + d * t; }
    friend std::ostream& operator<<(std::ostream& os, const Ray &r) {
        os << "[o=" << r.o << ", d=" << r.d << ", tMax=" << r.tMax << ", time=" << r.time << "]";
        return os;
    }
public:
    Point3f o;
    Vector3f d;
    mutable float tMax;
    float time;
    const Medium *medium;
};

#endif