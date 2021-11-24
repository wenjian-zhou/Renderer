#include "glass.h"

void Glass::ComputeScatteringFunctions(HitRecord *si, TransportMode mode) const {
    float eta = index;
    float rough = roughness;
    Spectrum R = Kr;
    Spectrum T = Kt;

    si->bsdf = new BSDF(si->normal, eta);

    if (R.IsBlack() && T.IsBlack()) return;

    bool isSpecular = (rough == 0);

    MicrofacetDistribution *distrib = new TrowbridgeReitzDistribution(rough, rough);

    if (!R.IsBlack()) {
        Fresnel *fresnel = new FresnelDielectric(1.f, eta);
        if (isSpecular) {
            BxDF *specularReflection = new SpecularReflection(R, fresnel);
            si->bsdf->Add(specularReflection);
        }
        else {
            BxDF *microfacetReflection = new MicrofacetReflection(R, distrib, fresnel);
            si->bsdf->Add(microfacetReflection);
        }
    }
    if (!T.IsBlack()) {
        if (isSpecular) {
            BxDF *specularTransmission = new SpecularTransmission(T, 1.f, eta, mode);
            si->bsdf->Add(specularTransmission);
        }
        else {
            BxDF *microfacetTransmission = new MicrofacetTransmission(T, distrib, 1.f, eta, mode);
            si->bsdf->Add(microfacetTransmission);
        }
    }
}