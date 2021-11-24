#include "path.h"

Spectrum PathIntegrator::Li(const Ray &r, const Scene &scene, Sampler &sampler) const {
    Spectrum L(0.f), beta(1.f);
    Ray ray(r);
    bool specularBounce = false;
    for (int bounces = 0; ; ++bounces) {
        HitRecord isect;
        bool foundIntersection = scene.Intersect(ray, isect);
        if (bounces == 0 || specularBounce) {
            if (foundIntersection)
                L += beta * isect.Le;
            else
                for (const auto &light : scene.lights) {
                    L += beta * light->Le(ray);
                }
        }

        if (!foundIntersection || bounces >= maxDepth)
            break;
        
        isect.mat_ptr->ComputeScatteringFunctions(&isect, TransportMode::Radiance);

        if (!isect.bsdf) {
            ray = Ray(ray.o, ray.d, ray.tMax, ray.time, ray.medium);
            bounces --;
            continue;
        }

        L += beta * UniformSampleOneLight(isect, scene, sampler);

        Vector3f wo = -ray.d, wi;
        float pdf;
        BxDFType flags;
        Spectrum f = isect.bsdf->Sample_f(wo, &wi, sampler.Next2D(), &pdf, BSDF_ALL, &flags);

        if (f.IsBlack() || pdf == 0.f)
            break;
        beta *= f * AbsDot(wi, isect.normal) / pdf;
        specularBounce = (flags & BSDF_SPECULAR) != 0;
        ray = Ray(isect.p, wi);

        if (bounces > 3) {
            float q = std::max((float).05, 1 - beta.y());
            if (random_double(0.f, 1.f) < q)
                break;
            beta /= 1 - q;
        }

        delete (isect.bsdf);
    }
    return L;
}