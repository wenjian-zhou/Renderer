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

        if (!isect.mat_ptr) {
            ray = Ray(isect.p + ray.d * 0.0001, ray.d, INF, 0.f, ray.medium);
            bounces --;
            continue;
        }
        
        isect.mat_ptr->ComputeScatteringFunctions(&isect, TransportMode::Radiance);

        //std::cout << UniformSampleOneLight(ray, isect, scene, sampler, false) << std::endl;
        L += beta * UniformSampleOneLight(ray, isect, scene, sampler, false);

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

        if (isect.bsdf) delete (isect.bsdf);
    }
    return L;
}