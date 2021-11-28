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
#include "../shapes/sphere.h"
#include "../materials/matte.h"
#include "../materials/glass.h"
#include "../integrators/path.h"
#include "../integrators/volpath.h"
#include "../medium/homogeneous.h"

void Renderer::Render() {

    std::vector<std::shared_ptr<Object>> objects; std::vector<std::shared_ptr<Light>> lights;

    auto white = std::make_shared<Matte>(Spectrum(.73f, .73f, .73f), 1.f);
    auto red = std::make_shared<Matte>(Spectrum(.65, .05, .05), 1.f);
    auto blue = std::make_shared<Matte>(Spectrum(.09, .09, .47), 1.f);
    auto green = std::make_shared<Matte>(Spectrum(.12, .45, .15), 1.f);
    auto black = std::make_shared<Matte>(Spectrum(.01, .01, .01), 1.f);
    Spectrum lightColor = 8.0f * Spectrum(0.747f+0.058f, 0.747f+0.258f, 0.747f) + 15.6f * Spectrum(0.740f+0.287f,0.740f+0.160f,0.740f) + 18.4f * Spectrum(0.737f+0.642f,0.737f+0.159f,0.737f);

    auto roughGlass = std::make_shared<Glass>(Spectrum(1.f), Spectrum(1.f), 0.0f, 1.5f);

    auto light = make_shared<XZRect>(213, 343, 227, 332, 554, nullptr);
    auto diffuseLight = make_shared<DiffuseAreaLight>(lightColor, 1, light, false);

    auto media = std::make_shared<HomogeneousMedium>(0.001, 0.0012, 0.0);
    /*
    std::string model = "../models/bunny/bunny.obj";
    std::string mtl_path = "../models/bunny/";
    TriangleMesh bunny = TriangleMesh(model, mtl_path, black);
    ObjectList list;
    for (int s = 0; s < bunny.Triangles.size(); s++) {
        list.add(make_shared<Triangle>(bunny.Triangles[s]));
    }
    */
    ObjectList list;
    list.add(std::make_shared<YZRect>(0, 555, 0, 555, 555, red));
    list.add(std::make_shared<YZRect>(0, 555, 0, 555, 0, green));
    list.add(std::make_shared<XZRect>(0, 555, 0, 555, 0, white));
    list.add(std::make_shared<XZRect>(0, 555, 0, 555, 555, white));
    list.add(std::make_shared<XYRect>(0, 555, 0, 555, 555, white));
    list.add(std::make_shared<XZRect>(213, 343, 227, 332, 554, nullptr));
    list.add(std::make_shared<Sphere>(Point3f(277, 210, 277), 150, white));

    // objects.push_back(std::make_shared<BVH>(list, 0, 1));
    objects.push_back(std::make_shared<BVH>(list, 0, 1));
    lights.push_back(diffuseLight);

    Scene scene(objects, lights);

    Point3f lookfrom(278, 278, -800);
    Point3f lookat(278, 278, 0);
    auto vfov = 40.0;
    Vector3f vup(0, 1, 0);
    auto dist_to_focus = 10.f;
    auto aperture = 0.0;
    int spp = 1024;

    camera cam(lookfrom, lookat, vup, vfov, 1.0, aperture, dist_to_focus, 0.f, 0.f);
    m_camera = std::make_shared<camera>(cam);

    Sampler sampler;
    auto path = std::make_shared<PathIntegrator>(50, nullptr, std::make_shared<Sampler>(sampler));
    auto volpath = std::make_shared<VolPathIntegrator>(50, nullptr, std::make_shared<Sampler>(sampler));
    integrator = volpath;

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
                ray.medium = media;
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