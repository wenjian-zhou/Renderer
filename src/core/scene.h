#ifndef SCENE_H
#define SCENE_H

#include "global.h"
#include "object.h"
#include "light.h"

class Scene {
public:
    Scene(std::vector<std::shared_ptr<Object>> objects, std::vector<std::shared_ptr<Light>> lights)
        : objects(objects), lights(lights) {}
public:
    std::vector<std::shared_ptr<Object>> objects;
    std::vector<std::shared_ptr<Light>> lights;
};

#endif