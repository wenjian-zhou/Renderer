#include "global.h"

#include "color.h"
#include "hittable_list.h"
#include "sphere.h"
#include "envmap.h"
#include "triangle.h"
#include "mesh_triangle.h"
#include "camera.h"
#include "material.h"
#include "moving_sphere.h"
#include "aarect.h"
#include "box.h"
#include "constant_medium.h"
#include "bvh.h"
#include "onb.h"
#include "pdf.h"

#include <iostream>

color ray_color(
    const ray &r,
    const color &background,
    const hittable &world,
    shared_ptr<hittable_list> &lights,
    int depth)
{
    hit_record rec;
    // If we've exceeded the ray bounce limit, no more light is gathered.
    if (depth <= 0)
        return color(0, 0, 0);

    // If the ray hits nothing, return the background color.
    if (!world.hit(r, 0.001, infinity, rec))
        return background;
    scatter_record srec;
    color emitted = rec.mat_ptr->emitted(r, rec, rec.u, rec.v, rec.p);
    if (!rec.mat_ptr->scatter(r, rec, srec))
        return emitted;

    if (srec.is_specular)
    {
        return srec.attenuation * ray_color(srec.specular_ray, background, world, lights, depth - 1);
    }

    auto light_ptr = make_shared<hittable_pdf>(lights, rec.p);
    mixture_pdf p(light_ptr, srec.pdf_ptr);

    ray scattered = ray(rec.p, p.generate(), r.time());
    auto pdf_val = p.value(scattered.direction());
    return emitted + srec.attenuation * rec.mat_ptr->eval(r, rec, scattered) * ray_color(scattered, background, world, lights, depth - 1) / pdf_val;
}

hittable_list test_envmap()
{
    hittable_list objects;

    auto red = make_shared<lambertian>(color(.65, .05, .05));
    auto white = make_shared<lambertian>(color(1.0, 1.0, 1.0));
    auto green = make_shared<lambertian>(color(.12, .45, .15));
    auto light = make_shared<diffuse_light>(color(15, 15, 15));
    auto checker = make_shared<lambertian>(make_shared<checker_texture>(color(0.2f, 0.2f, 0.2f), color(0.9f, 0.9f, 0.9f)));

    //objects.add(make_shared<yz_rect>(-10000, 9550, -10000, 9550, 955, white));
    //objects.add(make_shared<yz_rect>(-10000, 9550, -10000, 9550, -800, white));
    //objects.add(make_shared<flip_face>(make_shared<xz_rect>(0, 150, 0, 150, 554, light)));
    //objects.add(make_shared<xz_rect>(0, 555, 0, 555, 555, white));
    //objects.add(make_shared<xz_rect>(-10000, 10000, -10000, 10000, 0.0333099, checker));
    //objects.add(make_shared<xy_rect>(-10000, 10000, -10000, 10000, 3000, white));

    /*
    shared_ptr<hittable> box1 = make_shared<box>(point3(0, 0, 0), point3(165, 330, 165), white);
    box1 = make_shared<rotate_y>(box1, 15);
    box1 = make_shared<translate>(box1, vec3(265, 0, 295));
    objects.add(box1);
    */
    auto emat = make_shared<diffuse_light>(make_shared<image_texture>("../src/env4.jpg"));
    objects.add(make_shared<envmap>(point3(0,0,0), 100, emat));

    objects.add(make_shared<xz_rect>(-50, 50, -40, 30, -46.65495f, checker));

    auto gold = make_shared<conductor>(color(1.0f), 0.2, vec3(0.13100f, 0.42415f, 1.3831f), vec3(4.0624f, 2.4721f, 1.9155f));
    auto alu = make_shared<conductor>(color(1.0f), 0.001, vec3(1.9214f, 1.0152f, 0.63324f), vec3(8.1420f, 6.6273f, 5.4544f));
    //objects.add(make_shared<sphere>(point3(82+270, 90, 82+295), 90, gold));
    //objects.add(make_shared<sphere>(point3(0, 90, 0), 90, alu));
    std::string model = "../models/bunny/bunny.obj";
    mesh_triangle bunny = mesh_triangle(model, alu);

    hittable_list faces;
    for (size_t s = 0; s < bunny.triangles.size(); s++)
    {
        float scale = 500.0f;
        vec3 v0 = bunny.triangles[s].v0 * scale;
        vec3 v1 = bunny.triangles[s].v1 * scale;
        vec3 v2 = bunny.triangles[s].v2 * scale;

        v0 += vec3(15, -50, 0);
        v1 += vec3(15, -50, 0);
        v2 += vec3(15, -50, 0);

        faces.add(make_shared<triangle>(v0, v1, v2, alu));
    }
    objects.add(make_shared<bvh_node>(faces, 0, 1));

    return objects;
}

int main()
{

    // Image

    const auto aspect_ratio = 1.0 / 1.0;
    const int image_width = 600;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 2048;
    const int max_depth = 50;

    // World

    auto world = test_envmap();
    auto lights = make_shared<hittable_list>();
    lights->add(make_shared<xz_rect>(0, 150, 0, 150, 554, make_shared<material>()));
    // lights->add(make_shared<sphere>(point3(190, 90, 190), 90, make_shared<material>()));

    color background(0, 0, 0);

    // Camera

    point3 lookfrom(0, 20, 80);
    point3 lookat(0, 0, 0);
    vec3 vup(0, 1, 0);
    auto dist_to_focus = 1.0;
    auto aperture = 0.0;
    auto vfov = 80;
    auto time0 = 0.0;
    auto time1 = 1.0;

    camera cam(lookfrom, lookat, vup, vfov, aspect_ratio, aperture, dist_to_focus, time0, time1);

    // Render

    FILE *f = fopen("image.ppm", "w"); // Write image to PPM file.
    fprintf(f, "P3\n%d %d\n%d\n", image_width, image_height, 255);

    for (int j = image_height - 1; j >= 0; --j)
    {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i)
        {
            color pixel_color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; ++s)
            {
                auto u = (i + random_double()) / (image_width - 1);
                auto v = (j + random_double()) / (image_height - 1);
                ray r = cam.get_ray(u, v);
                r.dir = normalize(r.dir);
                pixel_color += ray_color(r, background, world, lights, max_depth);
            }
            // write_color(std::cout, pixel_color, samples_per_pixel);
            auto r = pixel_color.x();
            auto g = pixel_color.y();
            auto b = pixel_color.z();

            // Divide the color by the number of samples and gamma-correct for gamma=2.0.
            auto scale = 1.0 / samples_per_pixel;
            r = sqrt(scale * r);
            g = sqrt(scale * g);
            b = sqrt(scale * b);

            // Write the translated [0,255] value of each color component.
            fprintf(f, "%d %d %d ", static_cast<int>(256 * clamp(r, 0.0, 0.999)),
                    static_cast<int>(256 * clamp(g, 0.0, 0.999)),
                    static_cast<int>(256 * clamp(b, 0.0, 0.999)));
        }
    }

    std::cerr << "\nDone.\n";
}