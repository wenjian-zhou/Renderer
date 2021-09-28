#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"

#include <iostream>
#include <algorithm>

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

#endif