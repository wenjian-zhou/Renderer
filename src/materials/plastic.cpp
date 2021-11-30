#include "plastic.h"

void Plastic::ComputeScatteringFunctions(HitRecord *si, TransportMode mode) const {
    Spectrum kd = Kd;
    si->bsdf = std::make_shared<BSDF>(si->normal);
    if (!kd.IsBlack()) {
        auto lam = std::make_shared<LambertionReflection>(kd);
        si->bsdf->Add(lam);
    }

    Spectrum ks = Ks;
    if (!ks.IsBlack()) {
        auto fresnel = std::make_shared<FresnelDielectric>(1.f, 1.5f);
        auto distrib = std::make_shared<TrowbridgeReitzDistribution>(roughness, roughness);
        auto spec = std::make_shared<MicrofacetReflection>(ks, distrib, fresnel);
        si->bsdf->Add(spec);
    }
}