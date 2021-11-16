#include "../core/material.h"

class Glass : public Material {
public:
    Glass(const Spectrum &Kr, const Spectrum &Kt, const float &roughness, const float &index)
        : Kr(Kr), Kt(Kt), roughness(roughness), index(index) {}

    void ComputeScatteringFunctions(HitRecord *si, TransportMode mode) const;
private:
    Spectrum Kr, Kt;
    float roughness;
    float index;
};