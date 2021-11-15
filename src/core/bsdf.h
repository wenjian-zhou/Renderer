#ifndef BXDF_H
#define BXDF_H

#include "global.h"
#include "spectrum.h"
#include "vector.h"
#include "microfacet.h"

// ******************************
//  BSDF inline functions start
// ******************************

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
inline bool SameHemisphere(const Vector3f &w, const Vector3f &wp) {
    return w.z * wp.z > 0;
}
inline Vector3f Reflect(const Vector3f &wo, const Vector3f &n) {
    return -wo + 2 * Dot(wo, n) * n;
}
inline bool Refract(const Vector3f &wi, const Vector3f &n, float eta, Vector3f *wt) {
    float cosThetaI = Dot(wi, n);
    float sin2ThetaI = std::max(0.f, 1.f - cosThetaI * cosThetaI);
    float sin2ThetaT = eta * eta * sin2ThetaI;

    if (sin2ThetaT >= 1) return false;

    float cosThetaT = std::sqrt(1.f - sin2ThetaT);
    *wt = eta * (-wi) + (eta * cosThetaI - cosThetaT) * n;
    return true;
}

// ******************************
//   BSDF inline functions end
// ******************************

enum class TransportMode { Radiance, Importance };

// ******************************
//        Fresnel start
// ******************************

float FrDielectric(float cosThetaI, float etaI, float etaT);
Spectrum FrConductor(float cosThetaI, const Spectrum &etai, const Spectrum &etat, const Spectrum &k);

class Fresnel {
public:
    virtual ~Fresnel();
    virtual Spectrum Evaluate(float cosI) const = 0;
};

class FresnelConductor : public Fresnel {
public:
    FresnelConductor(const Spectrum &etaI, const Spectrum &etaT, const Spectrum &k) : etaI(etaI), etaT(etaT), k(k) {}
    Spectrum Evaluate(float cosThetaI) const;
private:
    Spectrum etaI, etaT, k;
};

class FresnelDielectric : public Fresnel {
public:
    Spectrum Evaluate(float cosThetaI) const;
    FresnelDielectric(float etaI, float etaT) : etaI(etaI), etaT(etaT) { }

private:
    float etaI, etaT;
};

// ******************************
//         Fresnel end
// ******************************

enum BxDFType {
    BSDF_REFLECTION = 1 << 0,
    BSDF_TRANSMISSION = 1 << 1,
    BSDF_DIFFUSE = 1 << 2,
    BSDF_GLOSSY = 1 << 3,
    BSDF_SPECULAR = 1 << 4,
    BSDF_ALL = BSDF_DIFFUSE | BSDF_GLOSSY | BSDF_SPECULAR | BSDF_REFLECTION | BSDF_TRANSMISSION,
};

class BxDF {
public:
    BxDF(BxDFType type) : type(type) {}
    bool MatchesFlags(BxDFType t) const {
        return (type & t) == type;
    }
    virtual Spectrum f(const Vector3f &wo, const Vector3f &wi) const = 0;
    virtual Spectrum Sample_f(const Vector3f &wo, Vector3f *wi,
                              const Point2f &sample, float *pdf) const;
    virtual Spectrum rho(const Vector3f &wo, int nSamples,
                         const Point2f &samples) const;
    virtual Spectrum rho(int nSamples, const Point2f &samples1,
                         const Point2f &samples2) const;
    virtual float Pdf(const Vector3f &wo, const Vector3f &wi) const;

public:
    const BxDFType type;
};

class ScaledBxDF : public BxDF {
public:
    ScaledBxDF(BxDF *bxdf, const Spectrum &scale) : BxDF(BxDFType(bxdf->type)), bxdf(bxdf), scale(scale) {}
    Spectrum rho(const Vector3f &w, int nSamples, const Point2f &samples) const {
        return scale * bxdf->rho(w, nSamples, samples);
    }
    Spectrum rho(int nSamples, const Point2f &samples1,
                 const Point2f &samples2) const {
                     return scale * bxdf->rho(nSamples, samples1, samples2);
    }
    Spectrum f(const Vector3f &wo, const Vector3f &wi) const;
    Spectrum Sample_f(const Vector3f &wo, Vector3f *wi, const Point2f &sample,
        float *pdf, BxDFType *sampledType) const;
public:
    BxDF *bxdf;
    Spectrum scale;
};

class SpecularReflection : public BxDF {
public:
    SpecularReflection(const Spectrum &R, Fresnel *fresnel)
        : BxDF(BxDFType(BSDF_REFLECTION | BSDF_SPECULAR)), R(R),
          fresnel(fresnel) {}
    Spectrum f(const Vector3f &wo, const Vector3f &wi) const {
        return Spectrum(0.f);
    }
    Spectrum Sample_f(const Vector3f &wo, Vector3f *wi, const Point2f &sample,
                      float *pdf, BxDFType *sampledType) const;
    float Pdf(const Vector3f &wo, const Vector3f &wi) const {
        return 0;
    }
private:
    const Spectrum R;
    const Fresnel *fresnel;
};

class SpecularTransmission : public BxDF {
public:
    SpecularTransmission(const Spectrum &T, float etaA, float etaB, TransportMode mode)
                    : BxDF(BxDFType(BSDF_TRANSMISSION | BSDF_SPECULAR)), T(T), etaA(etaA),
                      etaB(etaB), fresnel(etaA, etaB), mode(mode) {}
    Spectrum f(const Vector3f &wo, const Vector3f &wi) const {
        return Spectrum(0.f);
    }
    Spectrum Sample_f(const Vector3f &wo, Vector3f *wi, const Point2f &sample, 
                      float *pdf, BxDFType *sampledType) const;
    float Pdf(const Vector3f &wo, const Vector3f &wi) const {
        return 0;
    }
private:
    const Spectrum T;
    const float etaA, etaB;
    const FresnelDielectric fresnel;
    const TransportMode mode;
};

class FresnelSpecular : public BxDF {
public:
    FresnelSpecular(const Spectrum &R, const Spectrum &T, float etaA, float etaB, TransportMode mode)
                : BxDF(BxDFType(BSDF_REFLECTION | BSDF_TRANSMISSION | BSDF_SPECULAR)),
                  R(R), T(T), etaA(etaA), etaB(etaB), fresnel(etaA, etaB), mode(mode) {}

    Spectrum f(const Vector3f &wo, const Vector3f &wi) const {
        return Spectrum(0.f);
    }
    Spectrum Sample_f(const Vector3f &wo, Vector3f *wi, const Point2f &u,
                      float *pdf, BxDFType *sampledType) const;
    float Pdf(const Vector3f &wo, const Vector3f &wi) const {
        return 0;
    }
private:
    const Spectrum R, T;
    const float etaA, etaB;
    const FresnelDielectric fresnel;
    const TransportMode mode;
};

class LambertionReflection : public BxDF {
public:
    LambertionReflection(const Spectrum &R)
        : BxDF(BxDFType(BSDF_DIFFUSE | BSDF_REFLECTION)), R(R) {}

    Spectrum f(const Vector3f &wo, const Vector3f &wi) const;
    Spectrum rho(const Vector3f &, int, const Point2f &) const { return R; }
    Spectrum rho(int, const Point2f &, const Point2f &) const { return R; }
private:
    const Spectrum R;
};

class MicrofacetReflection : public BxDF {
public:
    MicrofacetReflection(const Spectrum &R, MicrofacetDistribution *distribution, Fresnel *fresnel)
        : BxDF(BxDFType(BSDF_REFLECTION | BSDF_GLOSSY)), R(R), distribution(distribution), fresnel(fresnel) {}
    
    Spectrum f(const Vector3f &wo, const Vector3f &wi) const;
    Spectrum Sample_f(const Vector3f &wo, Vector3f *wi, const Point2f &u, float *pdf, BxDFType *sampledType) const;
    float Pdf(const Vector3f &wo, const Vector3f &wi) const;
private:
    const Spectrum R;
    const MicrofacetDistribution *distribution;
    const Fresnel *fresnel;
};

class MicrofacetTransmission : public BxDF {
public:
    MicrofacetTransmission(const Spectrum &T, MicrofacetDistribution *distribution, float etaA, float etaB, TransportMode mode)
        : BxDF(BxDFType(BSDF_TRANSMISSION | BSDF_GLOSSY)), T(T), distribution(distribution), etaA(etaA), etaB(etaB), fresnel(etaA, etaB), mode(mode) {}
    
    Spectrum f(const Vector3f &wo, const Vector3f &wi) const;
    Spectrum Sample_f(const Vector3f &wo, Vector3f *wi, const Point2f &u, float *pdf, BxDFType *sampledType) const;
    float Pdf(const Vector3f &wo, const Vector3f &wi) const;
private:
    const Spectrum T;
    const MicrofacetDistribution *distribution;
    const float etaA, etaB;
    const FresnelDielectric fresnel;
    const TransportMode mode;
};

class BSDF {
public:
    BSDF(const Vector3f &n, float eta = 1) : n(n), eta(eta) {
        Vector3f temp = (std::fabs(n.x) > 0.9f) ? Vector3f(0, 1, 0) : Vector3f(1, 0, 0);
        sn = Normalize(Cross(n, temp));
        tn = Cross(n, sn);
    }
    void Add(BxDF *b) {
        bxdfs[nBxDFs++] = b;
    }
    Vector3f WorldToLocal(const Vector3f &v) const {
        return Vector3f(Dot(sn, v), Dot(tn, v), Dot(n, v));
    }
    Vector3f LocalToWorld(const Vector3f &v) const {
        return Vector3f(sn.x * v.x + tn.x * v.y + n.x * v.z,
                        sn.y * v.x + tn.y * v.y + n.y * v.z,
                        sn.z * v.x + tn.z * v.y + n.z * v.z);
    }
    Spectrum f(const Vector3f &woW, const Vector3f &wiW, BxDFType flags = BSDF_ALL) const;
    Spectrum Sample_f(const Vector3f &wo, Vector3f *wi, const Point2f &u, float *pdf, BxDFType type = BSDF_ALL, BxDFType *sampledType = nullptr) const;
    float Pdf(const Vector3f &wo, const Vector3f &wi, BxDFType flags = BSDF_ALL) const;
    const float eta;
private:
    ~BSDF() {}

    const Vector3f n;
    Vector3f sn, tn;
    int nBxDFs = 0;
    static constexpr int MaxBxDFs = 8;
    BxDF *bxdfs[MaxBxDFs];
};

#endif