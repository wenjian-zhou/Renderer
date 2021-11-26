#include "homogeneous.h"

Spectrum HomogeneousMedium::Tr(const Ray &ray, Sampler &sampler) const {
    return Exp(-sigma_t * std::min(ray.tMax * ray.d.Length(), MaxFloat));
}

Spectrum HomogeneousMedium::Sample(const Ray &ray, Sampler &sampler, HitRecord &it) const {
    int channel = std::min((int)(sampler.Next1D() * 3), 3 - 1);
    float dist = -std::log(1 - sampler.Next1D()) / sigma_t[channel];
    float t = std::min(dist / ray.d.Length(), ray.tMax);
    bool sampledMedium = t < ray.tMax;
    if (sampledMedium) {
        it.t = t;
        it.p = ray(t);
        it.wo = -ray.d;
        it.mediumRecord.phase = std::make_shared<HenyeyGreenstein>(g);
    }

    Spectrum Tr = Exp(-sigma_t * std::min(t, MaxFloat) * ray.d.Length());

    Spectrum density = sampledMedium ? (sigma_t * Tr) : Tr;
    float pdf = 0;
    for (int i = 0; i < 3; ++i) pdf += density[i];
    pdf *= 1 / (float)3;
    if (pdf == 0) {
        pdf = 1;
    }

    return sampledMedium ? (Tr * sigma_s / pdf) : (Tr / pdf);
}