#ifndef MATERIAL_PLASTIC_H
#define MATERIAL_PLASTIC_H

#include "../core/material.h"

class Plastic : public Material {
public:
    Plastic(const Spectrum &Kd, const Spectrum &Ks, const float &roughness)
        : Kd(Kd), Ks(Ks), roughness(roughness) {}
    void ComputeScatteringFunctions(HitRecord *si, TransportMode mode) const;
private:
    Spectrum Kd, Ks;
    float roughness;
};

#endif