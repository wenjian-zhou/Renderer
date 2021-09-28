#ifndef RENDERER_VPTINTEGRATOR_H
#define RENDERER_VPTINTEGRATOR_H

#include "../core/integrator.h"

class VPTIntegrator : public Integrator
{
    color castRay(const ray &r, const color &background, const hittable &world, int depth) const override;
};

#endif