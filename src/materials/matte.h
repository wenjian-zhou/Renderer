#ifndef MATERIAL_MATTE_H
#define MATERIAL_MATTE_H

#include "../core/material.h"

class Matte : public Material {
public:
    Matte(const Spectrum &Kd, const float &sigma) : Kd(Kd), sigma(sigma) {}
    virtual void ComputeScatteringFunctions(HitRecord *si, TransportMode mode) const override;
private:
    Spectrum Kd;
    float sigma;
};

#endif