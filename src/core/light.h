#ifndef LIGHT_H
#define LIGHT_H

#include "global.h"
#include "spectrum.h"
#include "scene.h"

enum class LightFlags : int { DeltaPosition = 1, DeltaDirection = 2, Area = 4, Infinite = 8 };

inline bool IsDelta(int flags) {
    return flags & (int)LightFlags::DeltaPosition ||
           flags & (int)LightFlags::DeltaDirection;
}

class Light {
public:
    Light(int flags) : flags(flags) {}
    virtual ~Light();
    virtual Spectrum Le(const Ray &ray) const;
    virtual Spectrum Sample_Li(const HitRecord &isect, const Point3f &u, Vector3f *wi, float *pdf, VisibilityTester *vis) const = 0;
public:
    const int flags;
};

struct VisibilityTester {
    VisibilityTester() {}
    VisibilityTester(const Point3f &p0, const Point3f &p1) : p0(p0), p1(p1) {}
    bool Unoccluded(const Scene &scene);
    Point3f p0, p1;
};

#endif