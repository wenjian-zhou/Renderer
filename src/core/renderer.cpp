#include "renderer.h"

#include "object.h"
#include "hittable_list.h"
#include "../accelerators/bvh.h"
#include "../core/light.h"
#include "../core/material.h"
#include "../lights/point.h"
#include "../lights/diffuse.h"
#include "../shapes/triangle.h"
#include "../shapes/aarect.h"
#include "../materials/matte.h"
#include "../integrators/path.h"

void Renderer::Render() {

    std::vector<std::shared_ptr<Object>> objects; std::vector<std::shared_ptr<Light>> lights;

    auto white = std::make_shared<Matte>(Spectrum(1.f, 1.f, 1.f), 1.f);
    auto red = std::make_shared<Matte>(Spectrum(.65, .05, .05), 1.f);

    auto light = make_shared<XZRect>(0, 1, 0, 1, 5, white);
    auto diffuseLight = make_shared<DiffuseAreaLight>(8.f, 1, light, false);

    std::string model = "../models/bunny/bunny.obj";
    TriangleMesh bunny = TriangleMesh(model, red);
    ObjectList list;
    for (int s = 0; s < bunny.Triangles.size(); s++) {
        list.add(make_shared<Triangle>(bunny.Triangles[s]));
    }
    objects.push_back(std::make_shared<YZRect>(-10, 10, -10, 10, 10, white));
    objects.push_back(std::make_shared<YZRect>(-10, 10, -10, 10, -10, white));
    objects.push_back(std::make_shared<XYRect>(-10, 10, -10, 10, 10.f, white));
    objects.push_back(std::make_shared<XYRect>(-10.f, 10.f, -10.f, 10.f, -10.f, white));
    objects.push_back(std::make_shared<XZRect>(-1.f, 1.f, -1.f, 1.f, (float)0.0333099, white));

    objects.push_back(std::make_shared<BVH>(list, 0, 1));
    lights.push_back(diffuseLight);

    Scene scene(objects, lights);

    Point3f lookfrom(0, 4, 8);
    Point3f lookat(-0.02, 0, -0.2);
    auto vfov = 1.3;
    Vector3f vup(0, 1, 0);
    auto dist_to_focus = 1.0;
    auto aperture = 0.0;
    int spp = 128;

    camera cam(lookfrom, lookat, vup, vfov, 1.0, aperture, dist_to_focus, 0.f, 0.f);
    m_camera = std::make_shared<camera>(cam);

    Sampler sampler;
    auto path = std::make_shared<PathIntegrator>(50, nullptr, std::make_shared<Sampler>(sampler));
    integrator = path;

    int image_height = 600, image_width = 600;

    FILE *f = fopen("image.ppm", "w"); // Write image to PPM file.
    fprintf(f, "P3\n%d %d\n%d\n", image_width, image_height, 255);

    for (int j = image_height - 1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            Spectrum pixel(0.f);
            for (int s = 0; s < spp; ++s) {
                auto u = (float)i / ((float)image_width - 1);
                auto v = (float)j / ((float)image_height - 1);
                Ray ray = m_camera->get_Ray(u, v);
                ray.d = Normalize(ray.d);
                pixel += integrator->Li(ray, scene, sampler);
            }
            auto r = pixel.r;
            auto g = pixel.g;
            auto b = pixel.b;
            // Divide the color by the number of samples and gamma-correct for gamma=2.0.
            auto scale = 1.0 / spp;
            r = sqrt(scale * r);
            g = sqrt(scale * g);
            b = sqrt(scale * b);

            fprintf(f, "%d %d %d ", static_cast<int>(256 * clamp(r, 0.0, 0.999)),
                    static_cast<int>(256 * clamp(g, 0.0, 0.999)),
                    static_cast<int>(256 * clamp(b, 0.0, 0.999)));
        }
    }
}