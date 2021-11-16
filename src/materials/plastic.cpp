#include "plastic.h"

void Plastic::ComputeScatteringFunctions(HitRecord *si, TransportMode mode) const {
    Spectrum kd = Kd;
    if (!kd.IsBlack()) {
        BxDF *lam = new LambertionReflection(kd);
        si->bsdf->Add(lam);
    }

    Spectrum ks = Ks;
    if (!ks.IsBlack()) {
        Fresnel *fresnel = new FresnelDielectric(1.f, 1.5f);
        MicrofacetDistribution *distrib = new TrowbridgeReitzDistribution(roughness, roughness);
        BxDF *spec = new MicrofacetReflection(ks, distrib, fresnel);
        si->bsdf->Add(spec);
    }
}