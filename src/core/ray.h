#ifndef RAY_H
#define RAY_H

#include "vector.h"

class Ray
{
public:
    Ray() : tMax(INF), time(0.f), medium(nullptr) {}
    Ray(const Point3f &o, const Vector3f &d, float tMax = INF, float time = 0.f, std::shared_ptr<Medium> medium = nullptr)
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
    std::shared_ptr<Medium> medium;
};

class RayDifferential : public Ray {
public:
    RayDifferential() { hasDifferentials = false; }
    RayDifferential(const Vector3f &o, const Vector3f &d,
                    float tMax = Infinity, float time = 0.f,
                    std::shared_ptr<Medium> medium = nullptr)
                : Ray(o, d, tMax, time, medium) {
                    hasDifferentials = false;
    }

    void ScaleDifferentials(float s) {
        rxOrigin = o + (rxOrigin - o) * s;
        ryOrigin = o + (ryOrigin - o) * s;
        rxDirection = o + (rxDirection - o) * s;
        ryDirection = o + (ryDirection - o) * s;
    }
public:
    bool hasDifferentials;
    Vector3f rxOrigin, ryOrigin;
    Vector3f rxDirection, ryDirection;
};

#endif