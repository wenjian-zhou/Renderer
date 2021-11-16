#include "../core/integrator.h"

class WhittedIntegrator : public SamplerIntegrator {
public:
    WhittedIntegrator(int maxDepth, std::shared_ptr<Camera> camera,
                      std::shared_ptr<Sampler> sampler)
                    : SamplerIntegrator(camera, sampler), maxDepth(maxDepth) {}
    
    Spectrum Li(const Ray &ray, const Scene &scene, const Sampler &sampler) const;
public:
    const int maxDepth;
};