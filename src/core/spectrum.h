#ifndef RENDERER_SPECTRUM_H
#define RENDERER_SPECTRUM_H

#include <assert.h>
#include <cmath>
#include <iostream>

#include "vector.h"

inline float SafeSqrt(const float &a) { return std::sqrt(std::max(0.f, a)); }

static const int nCIESamples = 471;
extern const float CIE_X[nCIESamples];
extern const float CIE_Y[nCIESamples];
extern const float CIE_Z[nCIESamples];
extern const float CIE_lambda[nCIESamples];
static const float CIE_Y_integral = 106.856895;

inline double Clamp(const double &v, const double &l, const double &r)
{
    return std::min(std::max(v, l), r);
}

inline double GammaCorrect(const double &v)
{
    return v < 0.0031308f ? 12.92f * v : 1.055f * std::pow(v, 1.0f / 2.4f) - 0.055f;
}

inline double InverseGammaCorrect(const double &v) 
{
    return v <= 0.04045f ? v / 12.92f : std::pow((v + 0.055f) / 1.055f, 2.4f);
}

inline void XYZToRGB(const double xyz[3], double rgb[3]) 
{
    rgb[0] = 3.240479f * xyz[0] - 1.537150f * xyz[1] - 0.498535f * xyz[2];
    rgb[1] = -0.969256f * xyz[0] + 1.875991f * xyz[1] + 0.041556f * xyz[2];
    rgb[2] = 0.055648f * xyz[0] - 0.204043f * xyz[1] + 1.057311f * xyz[2];
}

struct sRGB
{
    sRGB(double v = 0) : r(v), g(v), b(v) {}
    sRGB(double _r, double _g, double _b) : r(_r), g(_g), b(_b) {}

    bool IsBlack() const { return r == 0 && g == 0 && b == 0; }

    float r, g, b;
};

class RGBSpectrum
{
public:
    RGBSpectrum(float v = 0) : r(v), g(v), b(v) {}
    RGBSpectrum(float _r, float _g, float _b) : r(_r), g(_g), b(_b) {}
    RGBSpectrum(const float *v) : r(v[0]), g(v[1]), b(v[2]) {}
    RGBSpectrum(const std::string &filename);
    RGBSpectrum(const Vector3f &v) : r(std::fabs(v.x)), g(std::fabs(v.y)), b(std::fabs(v.z)) {}

    float operator [] (const uint32_t &i) const
    {
        if (i == 0) return r;
        else if (i == 1) return g;
        else return b;
    }
    float &operator [] (const uint32_t &i)
    {
        if (i == 0) return r;
        else if (i == 1) return g;
        else return b;
    }
    bool IsNaN() const { return std::isnan(r) | std::isnan(g) | std::isnan(b); }

    RGBSpectrum operator - () const { return RGBSpectrum(-r, -g, -b); }
    RGBSpectrum operator * (float v) const { return RGBSpectrum(r * v, g * v, b * v); }
    RGBSpectrum operator / (float v) const { assert(v != 0.f); return RGBSpectrum(r / v, g / v, b / v); }
    RGBSpectrum &operator /= (float v) { assert(v != 0); r /= v; g /= v; b /= v; return *this; }

    RGBSpectrum operator + (const RGBSpectrum &s) const { return RGBSpectrum(r + s.r, g + s.g, b + s.b); }
    RGBSpectrum &operator += (const RGBSpectrum &s) { r += s.r; g += s.g; b += s.b; return *this; }
    RGBSpectrum operator - (const RGBSpectrum &s) const { return RGBSpectrum(r - s.r, g - s.g, b - s.b); }
    RGBSpectrum operator * (const RGBSpectrum &s) const { return RGBSpectrum(r * s.r, g * s.g, b * s.b); }
    RGBSpectrum &operator *= (const RGBSpectrum &s) { r *= s.r; g *= s.g; b *= s.b; return *this; }
    RGBSpectrum operator / (const RGBSpectrum &s) const { return RGBSpectrum(r / s.r, g / s.g, b / s.b); }

    float Average() const { return float(r + g + b) / 3; }
    float y() const { return (r * 0.299) + (g * 0.587) + (b * 0.114); }
    bool IsBlack() const { return r == 0.f && g == 0.f && b == 0.f; }

    sRGB TosRGB() const { return sRGB(GammaCorrect(r), GammaCorrect(g), GammaCorrect(b)); }
    
    static RGBSpectrum FromSPD(const float *lambda, const float *val, const int &n);

    friend RGBSpectrum operator * (float v, const RGBSpectrum &s) { return RGBSpectrum(s.r * v, s.g * v, s.b * v); }
    friend RGBSpectrum Exp(const RGBSpectrum &s) { return RGBSpectrum(std::exp(s.r), std::exp(s.g), std::exp(s.b)); }
    friend RGBSpectrum Cos(const RGBSpectrum &s) { return RGBSpectrum(std::cos(s.r), std::cos(s.g), std::cos(s.b)); }
    friend RGBSpectrum Sqrt(const RGBSpectrum &s) { return RGBSpectrum(SafeSqrt(s.r), SafeSqrt(s.g), SafeSqrt(s.b)); }
    friend float MaxComponent(const RGBSpectrum &s) { return std::max(s.r, std::max(s.g, s.b)); }
    friend std::ostream &operator << (std::ostream &os, const RGBSpectrum &s)
    {
        os << s.r << ' ' << s.g << ' ' << s.b;
        return os;
    }
public:
    float r, g, b;
};

RGBSpectrum BlackBody(float t);
RGBSpectrum XYZToRGB(const RGBSpectrum &s);
RGBSpectrum Clamp(const RGBSpectrum &s, const RGBSpectrum &l, const RGBSpectrum &r);
RGBSpectrum ClampNegative(const RGBSpectrum &s);

inline void write_color(std::ostream &out, Spectrum pixel_color, int samples_per_pixel)
{
    auto r = pixel_color.r;
    auto g = pixel_color.g;
    auto b = pixel_color.b;

    // Replace NaN components with zero. See explanation in Ray Tracing: The Rest of Your Life.
    if (r != r) r = 0.0;
    if (g != g) g = 0.0;
    if (b != b) b = 0.0;

    // Divide the color by the number of samples and gamma-correct for gamma=2.0.
    auto scale = 1.0 / samples_per_pixel;
    r = sqrt(scale * r);
    g = sqrt(scale * g);
    b = sqrt(scale * b);

    // Write the translated [0,255] value of each color component.
    out << static_cast<int>(256 * clamp(r, 0.0, 0.999)) << ' '
        << static_cast<int>(256 * clamp(g, 0.0, 0.999)) << ' '
        << static_cast<int>(256 * clamp(b, 0.0, 0.999)) << '\n';
}

#endif