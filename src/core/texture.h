//
// Created by Wenjian Zhou on 2021/9/4.
//

#ifndef RENDERER_TEXTURE_H
#define RENDERER_TEXTURE_H

#include "global.h"
#include "global_stb_image.h"
#include "spectrum.h"

#include "perlin.h"

#include <iostream>

class Texture
{
public:
    virtual Spectrum Value(double u, double v, const Point3f &p) const = 0;
};

class Solid_Color : public Texture
{
public:
    Solid_Color() {}
    Solid_Color(Spectrum c) : color_value(c) {}

    Solid_Color(double red, double green, double blue)
    : Solid_Color(Spectrum(red, green, blue)) {}

    virtual Spectrum Value(double u, double v, const Vector3f &p) const override
    {
        return color_value;
    }

public:
    Spectrum color_value;
};

class Checker_Texture : public Texture
{
public:
    Checker_Texture() {}
    Checker_Texture(std::shared_ptr<Texture> _even, std::shared_ptr<Texture> _odd)
    : even(_even), odd(_odd) {}

    Checker_Texture(Spectrum c1, Spectrum c2)
    : even(std::make_shared<Solid_Color>(c1)), odd(std::make_shared<Solid_Color>(c2)) {}

    virtual Spectrum Value(double u, double v, const Point3f &p) const override
    {
        auto sines = sin(50.0f * p.x) * sin(50.0f * p.z);
        if (sines < 0)
        {
            return odd->Value(u, v, p);
        }
        else
        {
            return even->Value(u, v, p);
        }
    }

public:
    shared_ptr<Texture> odd;
    shared_ptr<Texture> even;
};

class Image_Texture : public Texture
{
public:
    const static int bytes_per_pixel = 3;

    Image_Texture()
            : data(nullptr), width(0), height(0), bytes_per_scanline(0) {}

    Image_Texture(const char* filename)
    {
        auto components_per_pixel = bytes_per_pixel;

        data = stbi_load(
                filename, &width, &height, &components_per_pixel, components_per_pixel);

        if (!data)
        {
            std::cerr << "ERROR: Could not load texture image file '" << filename << "'.\n";
            width = height = 0;
        }

        bytes_per_scanline = bytes_per_pixel * width;
    }

    ~Image_Texture()
    {
        delete data;
    }

    virtual Spectrum Value(double u, double v, const Vector3f& p) const override
    {
        // If we have no texture data, then return solid cyan as a debugging aid.
        if (data == nullptr)
            return Spectrum(0,1,1);

        // Clamp input texture coordinates to [0,1] x [1,0]
        u = clamp(u, 0.0, 1.0);
        v = 1.0 - clamp(v, 0.0, 1.0);  // Flip V to image coordinates

        auto i = static_cast<int>(u * width);
        auto j = static_cast<int>(v * height);

        // Clamp integer mapping, since actual coordinates should be less than 1.0
        if (i >= width)  i = width-1;
        if (j >= height) j = height-1;

        const auto color_scale = 1.0 / 255.0;
        auto pixel = data + j*bytes_per_scanline + i*bytes_per_pixel;

        return Spectrum(color_scale*pixel[0], color_scale*pixel[1], color_scale*pixel[2]);
    }

private:
    unsigned char *data;
    int width, height;
    int bytes_per_scanline;
};

class Noise_Texture : public Texture {
    public:
        Noise_Texture() {}
        Noise_Texture(double sc) : scale(sc) {}

        virtual Spectrum Value(double u, double v, const Point3f& p) const override {
            return Spectrum(1,1,1) * 0.5 * (1 + sin(scale*p.z + 10*noise.turb(p)));
        }

    public:
        Perlin noise;
        double scale;
};

#endif //RENDERER_TEXTURE_H
