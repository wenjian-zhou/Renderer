#include "glass.h"

void Glass::ComputeScatteringFunctions(HitRecord *si, TransportMode mode) const {
    float eta = index;
    float rough = roughness;
    Spectrum R = Kr;
    Spectrum T = Kt;

    si->bsdf = std::make_shared<BSDF>(si->normal, eta);

    if (R.IsBlack() && T.IsBlack()) return;

    bool isSpecular = (rough == 0);
    if (!R.IsBlack()) {
        auto fresnel = std::make_shared<FresnelDielectric>(1.f, eta);
        if (isSpecular) {
            auto specularReflection = std::make_shared<SpecularReflection>(R, fresnel);
            si->bsdf->Add(specularReflection);
        }
        else {
            auto distrib = std::make_shared<TrowbridgeReitzDistribution>(rough, rough);
            auto microfacetReflection = std::make_shared<MicrofacetReflection>(R, distrib, fresnel);
            si->bsdf->Add(microfacetReflection);
        }
    }
    if (!T.IsBlack()) {
        if (isSpecular) {
            auto specularTransmission = std::make_shared<SpecularTransmission>(T, 1.f, eta, mode);
            si->bsdf->Add(specularTransmission);
        }
        else {
            auto distrib = std::make_shared<TrowbridgeReitzDistribution>(rough, rough);
            auto microfacetTransmission = std::make_shared<MicrofacetTransmission>(T, distrib, 1.f, eta, mode);
            si->bsdf->Add(microfacetTransmission);
        }
    }
}