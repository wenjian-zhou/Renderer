#ifndef INTEGRATOR_PATH_H
#define INTEGRATOR_PATH_H

#include "../core/integrator.h"

class PathIntegrator : public Integrator {
public:
    PathIntegrator(int maxDepth, std::shared_ptr<Camera> camera,
                   std::shared_ptr<Sampler> sampler)
                : Integrator(camera, sampler), maxDepth(maxDepth) {}
    
    Spectrum Li(const Ray &ray, const Scene &scene, Sampler &sampler) const;
private:
    const int maxDepth;
};

#endif