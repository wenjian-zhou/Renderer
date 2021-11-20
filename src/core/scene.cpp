#include "scene.h"

bool Scene::Intersect(const Ray &ray, HitRecord &isect) const {
    HitRecord temp_isect;
    bool hit_anything = false;
    auto closest = INF;
    for (const auto &object : objects) {
        if (object->Intersect(ray, temp_isect)) {
            hit_anything = true;
            closest = temp_isect.t;
            isect = temp_isect;
        }
    }
    return hit_anything;
}

bool Scene::IntersectTr(Ray ray, Sampler &sampler, HitRecord &isect, Spectrum *Tr) const {
    *Tr = Spectrum(1.f);
    while (true) {
        bool hitSurface = Intersect(ray, isect);
        if (ray.medium)
            *Tr *= ray.medium->Tr(ray, sampler);
        if (!hitSurface)
            return false;
        if (isect.GetMaterial() != nullptr)
            return true;
        ray = Ray(isect.p, ray.d, ray.tMax, ray.time, isect.GetMedium(ray.d));
    }
}