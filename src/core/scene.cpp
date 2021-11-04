#include "scene.h"

bool Scene::Intersect(const Ray &ray, HitRecord &isect) const {
    HitRecord temp_isect;
    bool hit_anything;
    auto closest = INF;

    for (const auto &object : objects) {
        if (object->Intersect(ray, temp_isect)) {
            hit_anything = true;
            closest = temp_isect.t;
            isect = temp_isect;
        }
    }
    
    return hit_anything;
}