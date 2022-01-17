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
#include "../materials/metal.h"
#include "../materials/plastic.h"
#include "../integrators/path.h"
#include "../integrators/volpath.h"
#include "../medium/homogeneous.h"

void Renderer::Render() {

    std::vector<std::shared_ptr<Object>> objects; std::vector<std::shared_ptr<Light>> lights;

    auto white = std::make_shared<Matte>(Spectrum(.73f, .73f, .73f), 1.f);
    auto red = std::make_shared<Matte>(Spectrum(.75, .25, .25), 1.f);
    auto blue = std::make_shared<Matte>(Spectrum(.25, .25, .75), 1.f);
    auto green = std::make_shared<Matte>(Spectrum(.12, .45, .15), 1.f);
    auto black = std::make_shared<Matte>(Spectrum(.01, .01, .01), 1.f);
    auto metal_silver = std::make_shared<Metal>(Spectrum(0.041000, 0.059582, 0.040000), Spectrum(4.8025, 3.5974, 2.6484), 0.1);
    auto metal_gold = std::make_shared<Metal>(Spectrum(0.13100, 0.42415, 1.3831), Spectrum(4.0624, 2.4721, 1.9155), 0.1);
    auto plastic = std::make_shared<Plastic>(Spectrum(0.294, 0.f, 0.509), Spectrum(1.f, 1.f, 1.f), 0.01);
    Spectrum lightColor = 8.0f * Spectrum(0.747f+0.058f, 0.747f+0.258f, 0.747f) + 15.6f * Spectrum(0.740f+0.287f,0.740f+0.160f,0.740f) + 18.4f * Spectrum(0.737f+0.642f,0.737f+0.159f,0.737f);
    auto roughGlass = std::make_shared<Glass>(Spectrum(1.f), Spectrum(1.f), 0.0f, 1.5f);
    auto jadeGlass = std::make_shared<Glass>(Spectrum(1.f), Spectrum(1.f), 0.0f, 1.6f);
    auto thin_media = std::make_shared<HomogeneousMedium>(0.001, 0.0012, 0.0);
    auto jade_media = std::make_shared<HomogeneousMedium>(Spectrum(0.00053, 0.00123, 0.00213), Spectrum(0.00657, 0.00186, 0.009), 0.f);

    auto jade_medium = std::make_shared<MediumRecord>(jade_media, nullptr);

    auto light = make_shared<XZRect>(213, 343, 227, 332, 554, nullptr);
    auto diffuseLight = make_shared<DiffuseAreaLight>(lightColor, 1, light, false);
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
    list.add(std::make_shared<YZRect>(0, 555, 0, 555, 0, blue));
    list.add(std::make_shared<XZRect>(0, 555, 0, 555, 0, white));
    list.add(std::make_shared<XZRect>(0, 555, 0, 555, 555, white));
    list.add(std::make_shared<XYRect>(0, 555, 0, 555, 555, white));
    list.add(std::make_shared<XZRect>(213, 343, 227, 332, 554, nullptr));
    //list.add(std::make_shared<Sphere>(Point3f(138.75, 150, 138.75), 100, plastic, no_medium));
    //list.add(std::make_shared<Sphere>(Point3f(416.25, 150, 138.75), 100, roughGlass, medium));
    //list.add(std::make_shared<Sphere>(Point3f(138.75, 350, 416.25), 100, metal_gold, no_medium));
    //list.add(std::make_shared<Sphere>(Point3f(416.25, 350, 416.25), 100, white, no_medium));
    list.add(std::make_shared<Sphere>(Point3f(277, 210, 277), 100, jadeGlass, jade_medium));

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
    int spp = 16;

    camera cam(lookfrom, lookat, vup, vfov, 1.0, aperture, dist_to_focus, 0.f, 0.f);
    m_camera = std::make_shared<camera>(cam);

    Sampler sampler;
    auto path = std::make_shared<PathIntegrator>(50, nullptr, std::make_shared<Sampler>(sampler));
    auto volpath = std::make_shared<VolPathIntegrator>(50, nullptr, std::make_shared<Sampler>(sampler));
    integrator = volpath;

    int image_height = 600, image_width = 600;

    std::vector<Vector3f> framebuffer(image_height * image_width);
    int m = 0;
    
    omp_init_lock(&lock);
    omp_set_num_threads(16);
#pragma omp parallel for
    for (int j = image_height - 1; j >= 0; --j) {
        //std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
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

            framebuffer[(image_height - j - 1) * image_width + i] = Vector3f(r, g, b);
        }
        omp_set_lock(&lock);
        UpdateProgress((m++) / (float)image_height);
        omp_unset_lock(&lock);
    }
    UpdateProgress(1.);
    omp_destroy_lock(&lock);

    FILE *f = fopen("image.ppm", "w"); // Write image to PPM file.
    fprintf(f, "P3\n%d %d\n%d\n", image_width, image_height, 255);
    for (int i = 0; i < image_height * image_width; i++) {
        fprintf(f, "%d %d %d ", static_cast<int>(256 * clamp(framebuffer[i].x, 0.0, 0.999)),
                    static_cast<int>(256 * clamp(framebuffer[i].y, 0.0, 0.999)),
                    static_cast<int>(256 * clamp(framebuffer[i].z, 0.0, 0.999)));
    }
}