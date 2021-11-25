#include "volpath.h"

Spectrum VolPathIntegrator::Li(const Ray &r, const Scene &scene, Sampler &sampler) const {
    Spectrum L(0.f), beta(1.f);
    Ray ray(r);
    bool specularBounce = false;
    for (int bounces = 0; ; ++bounces) {
        HitRecord isect;
        bool foundIntersection = scene.Intersect(ray, isect);

        HitRecord mi;
        if (ray.medium) {
            beta *= ray.medium->Sample(ray, sampler, mi);
        }
        if (beta.IsBlack()) 
            break;

        if (mi.mediumRecord.IsValid()) {
            L += beta * UniformSampleOneLight(ray, mi, scene, sampler, true);
            Vector3f wo = -ray.d, wi;
            mi.mediumRecord.phase->Sample_p(wo, &wi, sampler.Next2D());
            ray = Ray(mi.p, wi, INF, 0.f, mi.GetMedium(wi));
            specularBounce = false;
        }
        else {
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

            L += beta * UniformSampleOneLight(ray, isect, scene, sampler, true);

            Vector3f wo = -ray.d, wi;
            float pdf;
            BxDFType flags;
            Spectrum f = isect.bsdf->Sample_f(wo, &wi, sampler.Next2D(), &pdf, BSDF_ALL, &flags);

            if (f.IsBlack() || pdf == 0.f)
                break;
            beta *= f * AbsDot(wi, isect.normal) / pdf;
            specularBounce = (flags & BSDF_SPECULAR) != 0;
            ray = Ray(isect.p, wi, INF, 0.0f, ray.medium);
        }
        if (bounces > 3) {
            float q = std::max((float).05, 1 - beta.y());
            if (sampler.Next1D() < q)
                break;
            beta /= 1 - q;
        }

        if (isect.bsdf) delete isect.bsdf;

        if (isect.mediumRecord.phase) {
            isect.mediumRecord.inside = isect.mediumRecord.outside = nullptr;
            isect.mediumRecord.phase = nullptr;
        }
    }

    return L;
}