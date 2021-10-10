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
    }

    virtual vec3 generate() const override
    {
        if (random_double() < 0.5)
            return p[0]->generate();
        else
            return p[1]->generate();
    }

public:
    shared_ptr<pdf> p[2];
};

#endif