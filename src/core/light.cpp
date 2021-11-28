#include "light.h"

Spectrum Light::Le(const Ray &ray) const {
    return Spectrum(0.f);
}

bool VisibilityTester::Unoccluded(const Scene &scene) const {
    Point3f origin = p0;
    Vector3f direction = p1 - p0;
    HitRecord isect;
    return !scene.Intersect(Ray(origin, direction, 1 - ShadowEpsilon), isect);
}

Spectrum VisibilityTester::Tr(const Ray &r, const Scene &scene, Sampler &sampler) const {
    Ray ray(p0, p1 - p0, 1.f - ShadowEpsilon, 0.f, r.medium);
    Spectrum Tr(1.f);
    while (true) {
        HitRecord isect;
        bool hitSurface = scene.Intersect(ray, isect);
        if (hitSurface && isect.mat_ptr != nullptr)
            return Spectrum(0.f);

        if (ray.medium) {
            //std::cout << ray.medium->Tr(ray, sampler) << std::endl;
            Tr *= ray.medium->Tr(ray, sampler);
        }

        if (!hitSurface)
            break;

        Vector3f dir = p1 - isect.p;
        ray = Ray(isect.p + dir * 0.0001, dir, 1.f - ShadowEpsilon, 0.f, r.medium);
    }

    return Tr;
}