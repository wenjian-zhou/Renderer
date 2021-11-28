#ifndef INTEGRATOR_H
#define INTEGRATOR_H

#include "bsdf.h"
#include "material.h"
#include "scene.h"
#include "camera.h"
#include "light.h"

Spectrum EstimateDirect(const Ray &r, const HitRecord &it, const Light &light, const Scene &scene, Sampler &sampler, bool handleMedia = false, bool specular = false);
Spectrum UniformSampleOneLight(const Ray &r, const HitRecord &it, const Scene &scene, Sampler &sampler, bool handleMedia = false);

class Integrator{
public:
    Integrator(std::shared_ptr<Camera> camera,
                      std::shared_ptr<Sampler> sampler)
                    : camera(camera), sampler(sampler) {}
    
    virtual Spectrum Li(const Ray &ray, const Scene &scene, Sampler &sampler) const = 0;
public:
    std::shared_ptr<Camera> camera;
    std::shared_ptr<Sampler> sampler;
};

#endif