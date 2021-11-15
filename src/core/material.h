#ifndef MATERIAL_H
#define MATERIAL_H

#include "global.h"

class Material {
public:
    virtual ~Material() {}
    virtual void ComputeScatteringFunctions(HitRecord *si, TransportMode mode) const = 0;
};

#endif