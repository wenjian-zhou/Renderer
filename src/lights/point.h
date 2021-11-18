#ifndef LIGHT_POINT_H
#define LIGHT_POINT_H

#include "../core/global.h"
#include "../core/light.h"
#include "../core/vector.h"

class PointLight : public Light {
public:
    PointLight(const Spectrum &I) : I(I), Light(1) {}
    Spectrum Sample_Li(const HitRecord &ref, const Point2f &u, Vector3f *wi, float *pdf, VisibilityTester *vis) const;
private:
    const Point3f pLight;
    const Spectrum I;
};

#endif