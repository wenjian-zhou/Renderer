#ifndef LIGHT_H
#define LIGHT_H

#include "global.h"
#include "spectrum.h"
#include "scene.h"

class Light {
public:
    virtual ~Light();
    virtual Spectrum Le(const Ray &ray) const;
    virtual Spectrum Sample_Li(const HitRecord &isect, const Point3f &u, Vector3f &wi, float &pdf, VisibilityTester &vis) const = 0;
};

struct VisibilityTester {
    VisibilityTester() {}
    VisibilityTester(const Point3f &p0, const Point3f &p1) : p0(p0), p1(p1) {}
    bool Unoccluded(const Scene &scene);
    Point3f p0, p1;
};

#endif