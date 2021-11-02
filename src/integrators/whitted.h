#ifndef WHITTED_H
#define WHITTED_H

#include "../core/global.h"
#include "../core/integrator.h"
#include "../core/spectrum.h"

class WhittedIntegrator : public SamplerIntegrator {
public:
    WhittedIntegrator(int maxDepth, std::shared_ptr<Camera> camera,
                      std::shared_ptr<Sampler> sampler)
                    : SamplerIntegrator(camera, sampler), maxDepth(maxDepth) {}
    
    Spectrum Li(const Ray &ray, const Scene &scene, const Sampler &sampler) const;
public:
    const int maxDepth;
};

#endif