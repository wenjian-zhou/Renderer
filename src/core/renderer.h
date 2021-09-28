#ifndef RENDERER_RENDER_H
#define RENDERER_RENDER_H

#include "global.h"
#include "camera.h"
#include "hittable_list.h"
#include "integrator.h"

class Render
{
public:
    Render();
    Render(std::shared_ptr<hittable_list> world, double aspect_ratio, int image_width, int spp,
            color background, point3 lookfrom, point3 lookat, double vfov, vec3 vup,
            double aperture, double dist_to_focus, RenderType renderType);
    void Start();

public:
    std::shared_ptr<Integrator> m_integrator;
    std::shared_ptr<hittable> m_world;
    camera *m_cam;
    double m_aspect_ratio;
    int m_image_width;
    int m_image_height;
    int m_spp;
    color m_background;
    point3 m_lookfrom;
    point3 m_lookat;
    double m_vfov;
};

#endif