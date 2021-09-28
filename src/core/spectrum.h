#ifndef RENDERER_SPECTRUM_H
#define RENDERER_SPECTRUM_H

#include <assert.h>
#include <cmath>
#include <iostream>

#include "color.h"
#include "vec3.h"

static const int nCIESamples = 471;
extern const float CIE_X[nCIESamples];
extern const float CIE_Y[nCIESamples];
extern const float CIE_Z[nCIESamples];
extern const float CIE_lambda[nCIESamples];
static const float CIE_Y_integral = 106.856895;

class RGBSpectrum
{
public:
    RGBSpectrum(double v = 0) : r(v), g(v), b(v) {}
    RGBSpectrum(double _r, double _g, double _b) : r(_r), g(_g), b(_b) {}
    RGBSpectrum(const double *v) : r(v[0]), g(v[1]), b(v[2]) {}
    RGBSpectrum(const std::string &filename);
    RGBSpectrum(const vec3 &v) : r(std::fabs(v.x())), g(std::fabs(v.y())), b(std::fabs(v.z())) {}

    double operator [] (const uint32_t &i) const
    {
        if (i == 0) return r;
        else if (i == 1) return g;
        else return b;
    }
    double &operator [] (const uint32_t &i)
    {
        if (i == 0) return r;
        else if (i == 1) return g;
        else return b;
    }
    bool IsNaN() const { return std::isnan(r) | std::isnan(g) | std::isnan(b); }

    RGBSpectrum operator - () const { return RGBSpectrum(-r, -g, -b); }
    RGBSpectrum operator * (double v) const { return RGBSpectrum(r * v, g * v, b * v); }
    RGBSpectrum operator / (double v) const { assert(v != 0.f); return RGBSpectrum(r / v, g / v, b / v); }
    RGBSpectrum &operator /= (double v) { assert(v != 0); r /= v; g /= v; b /= v; return *this; }

    RGBSpectrum operator + (const RGBSpectrum &s) const { return RGBSpectrum(r + s.r, g + s.g, b + s.b); }
    RGBSpectrum &operator += (const RGBSpectrum &s) { r += s.r; g += s.g; b += s.b; return *this; }
    RGBSpectrum operator - (const RGBSpectrum &s) const { return RGBSpectrum(r - s.r, g - s.g, b - s.b); }
    RGBSpectrum operator * (const RGBSpectrum &s) const { return RGBSpectrum(r * s.r, g * s.g, b * s.b); }
    RGBSpectrum &operator *= (const RGBSpectrum &s) { r *= s.r; g *= s.g; b *= s.b; return *this; }
    RGBSpectrum operator / (const RGBSpectrum &s) const { return RGBSpectrum(r / s.r, g / s.g, b / s.b); }

    double Average() const { return double(r + g + b) / 3; }
    double y() const { return (r * 0.299) + (g * 0.587) + (b * 0.114); }
    bool IsBlack() const { return r == 0 && g == 0 && b == 0; }

    sRGB TosRGB() const { return sRGB(GammaCorrect(r), GammaCorrect(g), GammaCorrect(b)); }
    
    static RGBSpectrum FromSPD(const double *lambda, const double *val, const int &n);
public:
    double r, g, b;
};

#endif