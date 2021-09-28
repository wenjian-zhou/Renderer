#ifndef RENDERER_INTEGRATOR_H
#define RENDERER_INTEGRATOR_H

#include "global.h"
#include "hittable.h"
#include "material.h"

class Integrator
{
public:
    virtual color castRay(const ray &r, const color &background, std::shared_ptr<hittable> world, int depth) = 0;
};

class PTIntegrator : public Integrator
{
public:
    PTIntegrator() {}
    virtual color castRay(const ray &r, const color &background, std::shared_ptr<hittable> world, int depth) override
    {
        hit_record rec;

        double RussianRoulette = 0.8;

        if (random_double(0, 1) > RussianRoulette)
        {
            return color(0, 0, 0);
        }

        if (!world->hit(r, 0.001, infinity, rec))
        {
            return background;
        }

        ray wi;
        color fr;
        color Le = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);

        if (!rec.mat_ptr->scatter(r, rec, fr, wi))
        {
            return Le;
        }

        return (Le + fr * castRay(wi, background, world, depth - 1)) / RussianRoulette;
    }
};

class VPTIntegrator : public Integrator
{
public:
    VPTIntegrator() {}
    virtual color castRay(const ray &r, const color &background, std::shared_ptr<hittable> world, int depth) override
    {}
};

#endif