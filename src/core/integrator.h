#ifndef INTEGRATOR_H
#define INTEGRATOR_H

#include "global.h"

#include "vector.h"
#include "scene.h"
#include "spectrum.h"
#include "ray.h"
#include "camera.h"
#include "sampler.h"
#include "material.h"
#include "light.h"

class Integrator {
public:
    virtual ~Integrator();
    virtual void Render(const Scene &scene) = 0;
};

class SamplerIntegrator : public Integrator {
public:
    SamplerIntegrator(std::shared_ptr<Camera> camera,
                      std::shared_ptr<Sampler> sampler)
                    : camera(camera), sampler(sampler) {}
    
    void Render(const Scene &scene);
    virtual Spectrum Li(const Ray &ray, const Scene &scene, const Sampler &sampler) const = 0;
public:
    std::shared_ptr<Camera> camera;
    std::shared_ptr<Sampler> sampler;
};

#endif