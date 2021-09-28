#ifndef RENDERER_PTINTEGRATOR_H
#define RENDERER_PTINTEGRATOR_H

#include "../core/integrator.h"

class PTIntegrator : public Integrator
{
    color castRay(const ray &r, const color &background, const hittable &world, int depth) const override;
};

#endif