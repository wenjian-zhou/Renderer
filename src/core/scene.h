#ifndef SCENE_H
#define SCENE_H

#include "object.h"
#include "light.h"
#include "medium.h"

class Scene {
public:
    Scene(std::vector<std::shared_ptr<Object>> objects, std::vector<std::shared_ptr<Light>> lights)
        : objects(objects), lights(lights) {}

    bool Intersect(const Ray &ray, HitRecord &isect) const;
    bool IntersectTr(Ray ray, Sampler &sampler, HitRecord &isect, Spectrum *transmittance) const;
public:
    std::vector<std::shared_ptr<Object>> objects;
    std::vector<std::shared_ptr<Light>> lights;
};

#endif