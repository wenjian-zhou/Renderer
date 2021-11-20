#include "matte.h"

void Matte::ComputeScatteringFunctions(HitRecord *si, TransportMode mode) const {
    Spectrum r = Kd;

    BSDF m_bsdf(si->normal, 1);
    si->bsdf = &m_bsdf;
    if (!r.IsBlack()) {
        BxDF *lam = new LambertionReflection(r);
        si->bsdf->Add(lam);
    }
}