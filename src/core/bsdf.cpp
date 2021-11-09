#include "bsdf.h"

float FrDielectric(float cosThetaI, float etaI, float etaT) {
    cosThetaI = Clamp(cosThetaI, -1, 1);
    bool entering = cosThetaI > 0.f;
    if (!entering) {
        std::swap(etaI, etaT);
        cosThetaI = std::abs(cosThetaI);
    }

    float sinThetaI = std::sqrt(std::max((float)0, 1 - cosThetaI * cosThetaI));
    float sinThetaT = etaI / etaT * sinThetaI;
    if (sinThetaT >= 1)
        return 1;

    float cosThetaT = std::sqrt(std::max((float)0, 1 - sinThetaT * sinThetaT));

    float Rparl = ((etaT * cosThetaI) - (etaI * cosThetaT)) /
                  ((etaT * cosThetaI) + (etaI * cosThetaT));
    float Rperp = ((etaI * cosThetaI) - (etaT * cosThetaT)) /
                  ((etaI * cosThetaI) + (etaT * cosThetaT));
    return (Rparl * Rparl + Rperp * Rperp) / 2;
}

Spectrum FrConductor(float cosThetaI, const Spectrum &etai,
                     const Spectrum &etat, const Spectrum &k) {
    cosThetaI = Clamp(cosThetaI, -1, 1);
    Spectrum eta = etat / etai;
    Spectrum etak = k / etai;

    float cosThetaI2 = cosThetaI * cosThetaI;
    float sinThetaI2 = 1. - cosThetaI2;
    Spectrum eta2 = eta * eta;
    Spectrum etak2 = etak * etak;

    Spectrum t0 = eta2 - etak2 - sinThetaI2;
    Spectrum a2plusb2 = Sqrt(t0 * t0 + 4 * eta2 * etak2);
    Spectrum t1 = a2plusb2 + cosThetaI2;
    Spectrum a = Sqrt(0.5f * (a2plusb2 + t0));
    Spectrum t2 = (float)2 * cosThetaI * a;
    Spectrum Rs = (t1 - t2) / (t1 + t2);

    Spectrum t3 = cosThetaI2 * a2plusb2 + sinThetaI2 * sinThetaI2;
    Spectrum t4 = t2 * sinThetaI2;
    Spectrum Rp = Rs * (t3 - t4) / (t3 + t4);

    return 0.5 * (Rp + Rs);
}

Spectrum ScaledBxDF::f(const Vector3f &wo, const Vector3f &wi) const {
    return scale * bxdf->f(wo, wi);
}

Spectrum FresnelConductor::Evaluate(float cosThetaI) const {
    return FrConductor(std::abs(cosThetaI), etaI, etaT, k);
}

Spectrum FresnelDielectric::Evaluate(float cosThetaI) const {
    return FrDielectric(cosThetaI, etaI, etaT);
}

Spectrum SpecularReflection::Sample_f(const Vector3f &wo, Vector3f &wi, const Point2f &sample, float &pdf, BxDFType *sampledType) const {
    wi = Vector3f(-wo.x, -wo.y, wo.z);
    pdf = 1;
    return fresnel->Evaluate(CosTheta(wi)) * R / AbsCosTheta(wi);
}

Spectrum LambertionReflection::f(const Vector3f &wo, const Vector3f &wi) const {
    return R * invPI;
}

Spectrum MicrofacetReflection::f(const Vector3f &wo, const Vector3f &wi) const {
    float cosThetaO = AbsCosTheta(wo), cosThetaI = AbsCosTheta(wi);
    Vector3f wh = wi + wo;
    if (cosThetaI == 0 || cosThetaO == 0) return Spectrum(0.f);
    if (wh.x == 0 && wh.y == 0 && wh.z == 0) return Spectrum(0.f);

    wh = Normalize(wh);
    Spectrum F = fresnel->Evaluate(Dot(wi, wh));
    return R * distribution->D(wh) * distribution->G(wo, wi) * F / (4 * cosThetaO * cosThetaI);
}

Spectrum MicrofacetTransmission::f(const Vector3f &wo,
                                   const Vector3f &wi) const {
    if (SameHemisphere(wo, wi)) return Spectrum(0.f);  // transmission only

    float cosThetaO = CosTheta(wo);
    float cosThetaI = CosTheta(wi);
    if (cosThetaI == 0 || cosThetaO == 0) return Spectrum(0.f);

    // Compute $\wh$ from $\wo$ and $\wi$ for microfacet transmission
    float eta = CosTheta(wo) > 0 ? (etaB / etaA) : (etaA / etaB);
    Vector3f wh = Normalize(wo + wi * eta);
    if (wh.z < 0) wh = -wh;

    // Same side?
    if (Dot(wo, wh) * Dot(wi, wh) > 0) return Spectrum(0.f);

    Spectrum F = fresnel.Evaluate(Dot(wo, wh));

    float sqrtDenom = Dot(wo, wh) + eta * Dot(wi, wh);
    float factor = (mode == TransportMode::Radiance) ? (1 / eta) : 1;

    return (Spectrum(1.f) - F) * T *
           std::abs(distribution->D(wh) * distribution->G(wo, wi) * eta * eta *
                    AbsDot(wi, wh) * AbsDot(wo, wh) * factor * factor /
                    (cosThetaI * cosThetaO * sqrtDenom * sqrtDenom));
}