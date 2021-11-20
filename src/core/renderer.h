#ifndef RENDERER_H
#define RENDERER_H

#include "scene.h"
#include "integrator.h"
#include "camera.h"

class Renderer {
public:
    Renderer() {}
    void Render();
public:
    std::shared_ptr<Sampler> sampler;
    std::shared_ptr<camera> m_camera;
    std::shared_ptr<Integrator> integrator;
};

#endif