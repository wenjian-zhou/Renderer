#include "matte.h"

void Matte::ComputeScatteringFunctions(HitRecord *si, TransportMode mode) const {
    Spectrum r = Kd;

    si->bsdf = std::make_shared<BSDF>(si->normal, 1);
    if (!r.IsBlack()) {
        auto lam = std::make_shared<LambertionReflection>(r);
        si->bsdf->Add(lam);
    }
}