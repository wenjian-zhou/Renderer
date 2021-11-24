#include "microfacet.h"

static void TrowbridgeReitzSample11(float cosTheta, float U1, float U2, float *slope_x, float *slope_y) {
    // special case (normal incidence)
    if (cosTheta > .9999) {
        float r = sqrt(U1 / (1 - U1));
        float phi = 6.28318530718 * U2;
        *slope_x = r * cos(phi);
        *slope_y = r * sin(phi);
        return;
    }

    float sinTheta =
        std::sqrt(std::max((float)0, (float)1 - cosTheta * cosTheta));
    float tanTheta = sinTheta / cosTheta;
    float a = 1 / tanTheta;
    float G1 = 2 / (1 + std::sqrt(1.f + 1.f / (a * a)));

    // sample slope_x
    float A = 2 * U1 / G1 - 1;
    float tmp = 1.f / (A * A - 1.f);
    if (tmp > 1e10) tmp = 1e10;
    float B = tanTheta;
    float D = std::sqrt(
        std::max(float(B * B * tmp * tmp - (A * A - B * B) * tmp), float(0)));
    float slope_x_1 = B * tmp - D;
    float slope_x_2 = B * tmp + D;
    *slope_x = (A < 0 || slope_x_2 > 1.f / tanTheta) ? slope_x_1 : slope_x_2;

    // sample slope_y
    float S;
    if (U2 > 0.5f) {
        S = 1.f;
        U2 = 2.f * (U2 - .5f);
    } else {
        S = -1.f;
        U2 = 2.f * (.5f - U2);
    }
    float z =
        (U2 * (U2 * (U2 * 0.27385f - 0.73369f) + 0.46341f)) /
        (U2 * (U2 * (U2 * 0.093073f + 0.309420f) - 1.000000f) + 0.597999f);
    *slope_y = S * z * std::sqrt(1.f + *slope_x * *slope_x);
}

static Vector3f TrowbridgeReitzSample(const Vector3f &wi, float alpha_x,
                                      float alpha_y, float U1, float U2) {
    // 1. stretch wi
    Vector3f wiStretched =
        Normalize(Vector3f(alpha_x * wi.x, alpha_y * wi.y, wi.z));

    // 2. simulate P22_{wi}(x_slope, y_slope, 1, 1)
    float slope_x, slope_y;
    TrowbridgeReitzSample11(CosTheta(wiStretched), U1, U2, &slope_x, &slope_y);

    // 3. rotate
    float tmp = CosPhi(wiStretched) * slope_x - SinPhi(wiStretched) * slope_y;
    slope_y = SinPhi(wiStretched) * slope_x + CosPhi(wiStretched) * slope_y;
    slope_x = tmp;

    // 4. unstretch
    slope_x = alpha_x * slope_x;
    slope_y = alpha_y * slope_y;

    // 5. compute normal
    return Normalize(Vector3f(-slope_x, -slope_y, 1.));
}

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

Vector3f TrowbridgeReitzDistribution::Sample_wh(const Vector3f &wo,
                                                const Point2f &u) const {
    Vector3f wh;
    if (!sampleVisibleArea) {
        float cosTheta = 0, phi = (2 * PI) * u[1];
        if (alphax == alphay) {
            float tanTheta2 = alphax * alphax * u[0] / (1.0f - u[0]);
            cosTheta = 1 / std::sqrt(1 + tanTheta2);
        } else {
            phi =
                std::atan(alphay / alphax * std::tan(2 * PI * u[1] + .5f * PI));
            if (u[1] > .5f) phi += PI;
            float sinPhi = std::sin(phi), cosPhi = std::cos(phi);
            const float alphax2 = alphax * alphax, alphay2 = alphay * alphay;
            const float alpha2 =
                1 / (cosPhi * cosPhi / alphax2 + sinPhi * sinPhi / alphay2);
            float tanTheta2 = alpha2 * u[0] / (1 - u[0]);
            cosTheta = 1 / std::sqrt(1 + tanTheta2);
        }
        float sinTheta =
            std::sqrt(std::max((float)0., (float)1. - cosTheta * cosTheta));
        wh = SphericalDirection(sinTheta, cosTheta, phi);
        if (!SameHemisphere(wo, wh)) wh = -wh;
    } else {
        bool flip = wo.z < 0;
        wh = TrowbridgeReitzSample(flip ? -wo : wo, alphax, alphay, u[0], u[1]);
        if (flip) wh = -wh;
    }
    return wh;
}

float MicrofacetDistribution::Pdf(const Vector3f &wo,
                                  const Vector3f &wh) const {
    if (sampleVisibleArea)
        return D(wh) * G1(wo) * AbsDot(wo, wh) / AbsCosTheta(wo);
    else
        return D(wh) * AbsCosTheta(wh);
}