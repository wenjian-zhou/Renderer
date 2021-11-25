#ifndef INTEGRATOR_VOLPATH_H
#define INTEGRATOR_VOLPATH_H

#include "../core/integrator.h"

class VolPathIntegrator : public Integrator {
public:
    VolPathIntegrator(int maxDepth, std::shared_ptr<Camera> camera,
                   std::shared_ptr<Sampler> sampler)
                   : Integrator(camera, sampler), maxDepth(maxDepth) {}
    Spectrum Li(const Ray &ray, const Scene &scene, Sampler &sampler) const;
    
private:
    const int maxDepth;
};

#endif