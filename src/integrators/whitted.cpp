#include "whitted.h"

Spectrum WhittedIntegrator::Li(const Ray &ray, const Scene &scene, const Sampler &sampler) const {
    Spectrum L(0.f);
    
    HitRecord isect;
    if (!scene.Intersect(ray, isect)) {
        for (const auto &light : scene.lights)
            L += light->Le(ray);
        return L;
    }

    Vector3f n = isect.normal;
    Vector3f wo = -ray.dir;

    L += isect.mat_ptr->Le(wo);

    for (const auto &light : scene.lights) {
        Vector3f wi;
        float pdf;
        VisibilityTester visibility;
        Spectrum Li = light->Sample_Li(isect, 0.f, wi, pdf, visibility);
        if (Li.IsBlack() || pdf == 0.f) continue;
        Spectrum f = isect.mat_ptr->Eval(wo, wi);
        if (!f.IsBlack() && visibility.Unoccluded(scene)) {
            L += f * Li * AbsDot(wi, n) / pdf;
        }
    }

    // TODO : add specular reflection and refraction

    return L;
}