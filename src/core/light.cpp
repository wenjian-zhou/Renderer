#include "light.h"

Spectrum Light::Le(const Ray &ray) const {
    return Spectrum(1.f);
}

bool VisibilityTester::Unoccluded(const Scene &scene) {
    Point3f origin = p0;
    Vector3f direction = Normalize(p1 - p0);
    HitRecord isect;
    return !scene.Intersect(Ray(origin, direction), &isect);
}