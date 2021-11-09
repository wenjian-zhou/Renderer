#include "microfacet.h"

float TrowbridgeReitzDistribution::D(const Vector3f &wh) const {
    float tan2Theta = Tan2Theta(wh);
    if (std::isinf(tan2Theta)) return 0.;
    const float cos4Theta = Cos2Theta(wh) * Cos2Theta(wh);
    float e = (Cos2Phi(wh) / (alphax * alphax) +
               Sin2Phi(wh) / (alphay * alphay)) * tan2Theta;
    return 1 / (PI * alphax * alphay * cos4Theta * (1 + e) * (1 + e));
}

float TrowbridgeReitzDistribution::Lambda(const Vector3f &w) const {
    float absTanTheta = std::abs(TanTheta(w));
    if (std::isinf(absTanTheta)) return 0.;
    float alpha = std::sqrt(Cos2Phi(w) * alphax * alphax +
                            Sin2Phi(w) * alphay * alphay);

    float alpha2Tan2Theta = (alpha * absTanTheta) * (alpha * absTanTheta);
    return (-1 + std::sqrt(1.f + alpha2Tan2Theta)) / 2;
}