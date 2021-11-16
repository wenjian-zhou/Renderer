#include "../core/material.h"

class Matte : public Material {
public:
    Matte(const Spectrum &Kd, const float &sigma) : Kd(Kd), sigma(sigma) {}
    void ComputeScatteringFunctions(HitRecord *si, TransportMode mode) const;
private:
    Spectrum Kd;
    float sigma;
};