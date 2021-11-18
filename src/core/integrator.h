#ifndef INTEGRATOR_H
#define INTEGRATOR_H

#include "global.h"

#include "vector.h"
#include "scene.h"
#include "spectrum.h"
#include "ray.h"
#include "camera.h"
#include "sampler.h"
#include "bsdf.h"
#include "material.h"
#include "light.h"
#include "record.h"

Spectrum EstimateDirect(const HitRecord &it, const Light &light, const Scene &scene, Sampler &sampler, bool handleMedia, bool specular);
Spectrum UniformSampleOneLight(const HitRecord &it, const Scene &scene, Sampler &sampler);

class Integrator {
public:
    virtual ~Integrator();
};

class SamplerIntegrator : public Integrator {
public:
    SamplerIntegrator(std::shared_ptr<Camera> camera,
                      std::shared_ptr<Sampler> sampler)
                    : camera(camera), sampler(sampler) {}
    
    virtual Spectrum Li(const Ray &ray, const Scene &scene, Sampler &sampler) const = 0;
public:
    std::shared_ptr<Camera> camera;
    std::shared_ptr<Sampler> sampler;
};

#endif