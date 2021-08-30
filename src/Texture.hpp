#ifndef TEXTURE_H
#define TEXTURE_H

#include "global.hpp"

class Texture
{
public:
    virtual Color value(float u, float v, const Point3 &p) const = 0;
};

class SolidColor : public Texture
{
public:
    SolidColor() {}
    SolidColor(Color c) : color_value(c) {}

    SolidColor(float red, float green, float blue)
        : SolidColor(Color(red, green, blue)) {}
    
    virtual Color value(float u, float v, const Vector3f &p) const override
    {
        return color_value;
    }

public:
    Color color_value;
};

#endif