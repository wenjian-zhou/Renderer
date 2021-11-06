#ifndef RENDERER_H
#define RENDERER_H

#include "global.h"
#include "vector.h"
#include "spectrum.h"
#include "scene.h"
#include "integrator.h"
#include "camera.h"
#include "sampler.h"

class Renderer {
public:
    std::shared_ptr<Sampler> sampler;
    std::shared_ptr<Camera> camera;
    std::shared_ptr<Scene> scene;
    std::shared_ptr<Integrator> integrator;
};

#endif