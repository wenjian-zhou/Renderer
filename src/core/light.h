#ifndef LIGHT_H
#define LIGHT_H

#include "spectrum.h"
#include "record.h"
#include "ray.h"
#include "scene.h"

enum class LightFlags : int { DeltaPosition = 1, DeltaDirection = 2, Area = 4, Infinite = 8 };

inline bool IsDelta(int flags) {
    return flags & (int)LightFlags::DeltaPosition ||
           flags & (int)LightFlags::DeltaDirection;
}

class Light {
public:
    Light(int flags) : flags(flags) {}
    virtual Spectrum Le(const Ray &ray) const;
    virtual Spectrum Sample_Li(const HitRecord &isect, const Point2f &u, Vector3f *wi, float *pdf, VisibilityTester *vis) const = 0;
    virtual float Pdf_Li(const HitRecord &ref, const Vector3f &wi) const = 0;
    virtual Spectrum L(const HitRecord &intr, const Vector3f &w) const = 0;
public:
    const int flags;
};

struct VisibilityTester {
    VisibilityTester() {}
    VisibilityTester(const Point3f &p0, const Point3f &p1) : p0(p0), p1(p1) {}
    bool Unoccluded(const Scene &scene) const;
    Spectrum Tr(const Ray &ray, const Scene &scene, Sampler &sampler) const;
    Point3f p0, p1;
};

class AreaLight: public Light {
public:
    AreaLight(int nSamples) : Light(4) {}
    virtual Spectrum L(const HitRecord &intr, const Vector3f &w) const = 0;
};

#endif