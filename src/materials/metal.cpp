#include "metal.h"

void Metal::ComputeScatteringFunctions(HitRecord *si, TransportMode mode) const {
    si->bsdf = std::make_shared<BSDF>(si->normal);

    auto frMf = std::make_shared<FresnelConductor>(1., eta, k);
    auto distrib = std::make_shared<TrowbridgeReitzDistribution>(roughness, roughness);
    auto metal = std::make_shared<MicrofacetReflection>(1., distrib, frMf);

    si->bsdf->Add(metal);
}