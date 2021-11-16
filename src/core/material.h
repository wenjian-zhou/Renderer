#ifndef MATERIAL_H
#define MATERIAL_H

#include "global.h"
#include "spectrum.h"
#include "bsdf.h"
#include "record.h"

class Material {
public:
    virtual ~Material() {}
    virtual void ComputeScatteringFunctions(HitRecord *si, TransportMode mode) const = 0;
};

#endif