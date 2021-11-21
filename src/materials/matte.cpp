#include "matte.h"

void Matte::ComputeScatteringFunctions(HitRecord *si, TransportMode mode) const {
    Spectrum r = Kd;

    si->bsdf = new BSDF(si->normal, 1);
    if (!r.IsBlack()) {
        BxDF *lam = new LambertionReflection(r);
        si->bsdf->Add(lam);
    }
}