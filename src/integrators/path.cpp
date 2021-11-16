#include "path.h"

Spectrum PathIntegrator::Li(const Ray &r, const Scene &scene, Sampler &sampler) const {
    Spectrum L(0.f), beta(1.f);
    Ray ray(r);
    bool specularBounce = false;
    for (int bounces = 0; ; ++bounces) {
        HitRecord isect;
        bool foundIntersection = scene.Intersect(ray, &isect);

        if (bounces == 0 || specularBounce) {
            if (foundIntersection)
                L += beta * isect.Le;
            else
                for (const auto &light : scene.lights)
                    L += beta * light->Le(ray);
        }

        if (!foundIntersection || bounces >= maxDepth)
            break;

        isect.mat_ptr->ComputeScatteringFunctions(&isect, TransportMode::Radiance);
        if (!isect.bsdf) {
            ray = Ray(ray.o, ray.d, ray.tMax, ray.time, ray.medium);
            bounces --;
            continue;
        }
    }

}