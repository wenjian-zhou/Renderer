#ifndef RENDERER_INTEGRATOR_H
#define RENDERER_INTEGRATOR_H

#include "../core/global.h"

#include "../core/color.h"
#include "../core/ray.h"
#include "../core/hittable.h"
#include "../core/material.h"

class Integrator
{
    virtual color castRay(const ray &r, const color &background, const hittable &world, int depth) const;
};

#endif