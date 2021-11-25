#include "light.h"

Spectrum Light::Le(const Ray &ray) const {
    return Spectrum(0.f);
}

bool VisibilityTester::Unoccluded(const Scene &scene) const {
    Point3f origin = p0;
    Vector3f direction = Normalize(p1 - p0);
    float t = (p1 - p0).Length();
    HitRecord isect;
    return !scene.Intersect(Ray(origin, direction, t - 0.0001), isect);
}

Spectrum VisibilityTester::Tr(const Ray &r, const Scene &scene, Sampler &sampler) const {
    Ray ray(p0, Normalize(Vector3f(p1 - p0)), INF, 0.f, r.medium);
    Spectrum Tr(1.f);
    while (true) {
        HitRecord isect;
        bool hitSurface = scene.Intersect(ray, isect);
        if (hitSurface && isect.GetMaterial() != nullptr)
            return Spectrum(0.f);

        if (ray.medium) {
            //std::cout << ray.tMax << std::endl;
            Tr *= ray.medium->Tr(ray, sampler);
        }

        if (!hitSurface)
            break;

        Vector3f dir = Normalize(p1 - p0);
        ray = Ray(isect.p + dir * 0.0001, dir, 0.9999, 0.f, r.medium);
    }

    return Tr;
}