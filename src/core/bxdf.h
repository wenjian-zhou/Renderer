#ifndef BXDF_H
#define BXDF_H

#include "global.h"
#include "spectrum.h"
#include "vector.h"

inline float CosTheta(const Vector3f &w) { return w.z; }
inline float Cos2Theta(const Vector3f &w) { return w.z * w.z; }
inline float AbsCosTheta(const Vector3f &w) { return std::abs(w.z); }
inline float Sin2Theta(const Vector3f &w) {
    return std::max((float)0, (float)1 - Cos2Theta(w));
}
inline float SinTheta(const Vector3f &w) {
    return std::sqrt(Sin2Theta(w));
}
inline float TanTheta(const Vector3f &w) {
    return SinTheta(w) / CosTheta(w);
}
inline float Tan2Theta(const Vector3f &w) {
    return Sin2Theta(w) / Cos2Theta(w);
}
inline float CosPhi(const Vector3f &w) {
    float sinTheta = SinTheta(w);
    return (sinTheta == 0) ? 1 : Clamp(w.x / sinTheta, -1, 1);
}
inline float SinPhi(const Vector3f &w) {
    float sinTheta = SinTheta(w);
    return (sinTheta == 0) ? 0 : Clamp(w.y / sinTheta, -1, 1);
}
inline float Cos2Phi(const Vector3f &w) {
    return CosPhi(w) * CosPhi(w);
}
inline float Sin2Phi(const Vector3f &w) {
    return SinPhi(w) * SinPhi(w);
}
inline float CosDPhi(const Vector3f &wa, const Vector3f &wb) {
    return Clamp((wa.x * wb.x + wa.y * wb.y) / 
                  std::sqrt((wa.x * wa.x + wa.y * wa.y) * 
                             (wb.x * wb.x + wb.y * wb.y)), -1, 1);
}

class BxDF {
public:
    virtual Spectrum f(const Vector3f &wo, const Vector3f &wi) const = 0;
    virtual Spectrum Sample_f(const Vector3f &wo, Vector3f &wi,
                              const Point2f &sample, float &pdf) const;
};

#endif