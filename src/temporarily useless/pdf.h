#ifndef PDF_H
#define PDF_H

#include "onb.h"
#include "vec3.h"

class pdf
{
public:
    virtual ~pdf() {}

    virtual double value(const vec3 &direction) const = 0;
    virtual vec3 generate() const = 0;
};

class average_pdf : public pdf
{
public:
    average_pdf(const vec3 &w) { normal = w; }
    virtual double value(const vec3 &direction) const override 
    {
        return 0.5 / pi;
    }
    virtual vec3 generate() const override 
    {
        return unit_vector(random_in_hemisphere(normal));
    }
public:
    vec3 normal;
};

class cosine_pdf : public pdf
{
public:
    cosine_pdf(const vec3 &w) { uvw.build_from_w(w); }

    virtual double value(const vec3 &direction) const override
    {
        auto cosine = dot(unit_vector(direction), uvw.w());
        return (cosine <= 0) ? 0 : cosine / pi;
    }

    virtual vec3 generate() const override
    {
        return uvw.local(random_cosine_direction());
    }

public:
    onb uvw;
};

class hittable_pdf : public pdf
{
public:
    hittable_pdf(shared_ptr<hittable> p, const point3 &origin) : ptr(p), o(origin) {}

    virtual double value(const vec3 &direction) const override
    {
        return ptr->pdf_value(o, direction);
    }

    virtual vec3 generate() const override
    {
        return ptr->random(o);
    }

public:
    point3 o;
    shared_ptr<hittable> ptr;
};

class mixture_pdf : public pdf
{
public:
    mixture_pdf(shared_ptr<pdf> p0, shared_ptr<pdf> p1)
    {
        p[0] = p0;
        p[1] = p1;
    }

    virtual double value(const vec3 &direction) const override
    {
        return 0.5 * p[0]->value(direction) + 0.5 * p[1]->value(direction);
        /*
        float weight0 = p[0]->value(direction) / (p[0]->value(direction) + p[1]->value(direction));
        float weight1 = p[1]->value(direction) / (p[0]->value(direction) + p[1]->value(direction));
        return weight0 * p[0]->value(direction) + weight1 * p[1]->value(direction);
        */
    }

    virtual vec3 generate() const override
    {
        
        if (random_double() < 0.5)
            return normalize(p[0]->generate());
        else
            return normalize(p[1]->generate());
        
    }

public:
    shared_ptr<pdf> p[2];
};

class GGX_pdf : public pdf
{
public:
    GGX_pdf(const vec3 &_wo, const vec3 &normal, float alpha) { wo = _wo; uvw.build_from_w(normal); alphaG = alpha; }
    
    virtual double value(const vec3 &direction) const
    {
        vec3 wm = unit_vector(unit_vector(direction) + wo);
        // std::cout << dot(wm, uvw.axis[2]) << std::endl;
        float cosTheta_m = dot(wm, uvw.axis[2]);
        float theta_m = std::acos(cosTheta_m);
        float cosTheta2 = pow(std::cos(theta_m), 2.0);
        float tanTheta2 = pow(std::tan(theta_m), 2.0);
        float root = alphaG / (cosTheta2 * (alphaG * alphaG + tanTheta2));
        float invPi = 1.0f / pi;
        return invPi * root * root * cosTheta_m / (4 * dot(direction, wm));
    }

    virtual vec3 generate() const
    {
        float x, y, z;
        
        float x1 = random_double(0, 0.999f), x2 = random_double(0, 0.999f);
        float theta = std::atan(alphaG * safe_sqrt(x1) / safe_sqrt(1 - x1));
        float kesai = 2 * pi * x2;

        z = std::cos(theta);
        x = std::sin(theta) * std::cos(kesai);
        y = std::sin(theta) * std::sin(kesai);
        vec3 _wm = vec3(x, y, z);
        // std::cout << dot(uvw.local(_wm), uvw.axis[2]) << std::endl;
        _wm = uvw.local(_wm);
        vec3 wi = 2.0f * (dot(_wm, wo)) * _wm - wo;
        // std::cout << dot(wo, _wm) << std::endl;
        return wi;
    }
public:
    onb uvw;
    float alphaG;
    vec3 wo;
};

#endif