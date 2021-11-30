#ifndef MATERIAL_METAL_H
#define MATERIAL_METAL_H

#include "../core/material.h"

class Metal : public Material {
public:
    Metal(const Spectrum &eta, const Spectrum &k, float roughness)
        : eta(eta), k(k), roughness(roughness) {}
    
    void ComputeScatteringFunctions(HitRecord *si, TransportMode mode) const;
private:
    const Spectrum eta, k;
    float roughness;
};

#endif