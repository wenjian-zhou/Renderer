#ifndef MEDIUM_H
#define MEDIUM_H

#include "global.h"
#include "vector.h"
#include "spectrum.h"
#include "ray.h"

class PhaseFunction {
public:
    virtual float p(const Vector3f &wo, const Vector3f &wi) const = 0;
    virtual float Sample_p(const Vector3f &wo, Vector3f &wi, const Point2f &u) const = 0;
};

inline float PhaseHG(float cosTheta, float g) {
    float denom = 1 + g * g + 2 * g * cosTheta;
    return Inv4Pi * (1 - g * g) / (denom * std::sqrt(denom));
}

class HenyeyGreenstein : public PhaseFunction {
public:
    HenyeyGreenstein(float g) : g(g) {}
    float p(const Vector3f &wo, const Vector3f &wi) const;
    float Sample_p(const Vector3f &wo, Vector3f &wi, const Point2f &sample) const;
private:
    const float g;
};

class Medium {
public:
    virtual ~Medium() {}
    virtual Spectrum Tr(const Ray &ray, Sampler &sampler) const = 0;
    virtual Spectrum Sample(const Ray &ray, Sampler &sampler, MediumRecord *mi) const = 0;
};

#endif