<<<<<<< HEAD
<<<<<<< HEAD
#include "global.hpp"

#include "Color.hpp"
#include "HittableList.hpp"
#include "Sphere.hpp"
<<<<<<< HEAD
#include "Camera.hpp"
#include "Material.hpp"
=======
#include "Vector.hpp"
#include "Color.hpp"
>>>>>>> parent of 51fe3f2 (add ray, sphere, hit class)
=======
>>>>>>> parent of 70be2a2 (update camera and material)

=======
>>>>>>> parent of d8f8d70 (add vector and color class)
#include <iostream>

<<<<<<< HEAD
<<<<<<< HEAD
Color castRay(const Ray &r, const Hittable &world, int depth)
=======
Color castRay(const Ray &r, const Hittable &world)
>>>>>>> parent of 70be2a2 (update camera and material)
{
    HitRecord rec;
    if (world.hit(r, 0, infinity, rec))
    {
        return 0.5 * (rec.normal + Color(1, 1, 1));
    }
    Vector3f unit_direction = normalized(r.direction());
    auto t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * Color(1.0, 1.0, 1.0) + t * Color(0.5, 0.7, 1.0);
}

<<<<<<< HEAD
HittableList randomScene() {
    HittableList world;

    auto ground_material = make_shared<Lambertian>(Color(0.5, 0.5, 0.5));
    world.add(make_shared<Sphere>(Point3(0,-1000,0), 1000, ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = randomFloat();
            Point3 center(a + 0.9*randomFloat(), 0.2, b + 0.9*randomFloat());

            if ((center - Point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<Material> Sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = Color::random() * Color::random();
                    Sphere_material = make_shared<Lambertian>(albedo);
                    world.add(make_shared<Sphere>(center, 0.2, Sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = Color::random(0.5, 1);
                    auto fuzz = randomFloat(0, 0.5);
                    Sphere_material = make_shared<Metal>(albedo, fuzz);
                    world.add(make_shared<Sphere>(center, 0.2, Sphere_material));
                } else {
                    // glass
                    Sphere_material = make_shared<Dielectric>(1.5);
                    world.add(make_shared<Sphere>(center, 0.2, Sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<Dielectric>(1.5);
    world.add(make_shared<Sphere>(Point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<Lambertian>(Color(0.4, 0.2, 0.1));
    world.add(make_shared<Sphere>(Point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<Metal>(Color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<Sphere>(Point3(4, 1, 0), 1.0, material3));

    return world;
}

=======
>>>>>>> parent of 51fe3f2 (add ray, sphere, hit class)
=======
>>>>>>> parent of 70be2a2 (update camera and material)
int main()
{
    // Image

<<<<<<< HEAD
<<<<<<< HEAD
    const auto aspect_ratio = 3.0 / 2.0;
    const int image_width = 1200;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int spp = 500;
    const int max_depth = 50;
=======
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
>>>>>>> parent of 70be2a2 (update camera and material)

    // World
    HittableList world;
    world.add(make_shared<Sphere>(Point3(0, 0, -1), 0.5));
    world.add(make_shared<Sphere>(Point3(0, -100.5, -1), 100));

    // Camera

<<<<<<< HEAD
    Point3 lookfrom(13, 2, 3);
    Point3 lookat(0, 0, 0);
    Vector3f vup(0, 1, 0);
    auto dist_to_focus = 10.0;
    auto aperture = 0.1;
    Camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);
=======
    const int image_width = 256;
    const int image_height = 256;
>>>>>>> parent of 51fe3f2 (add ray, sphere, hit class)
=======
    auto viewport_height = 2.0;
    auto viewport_width = aspect_ratio * viewport_height;
    auto focal_length = 1.0;

    auto origin = Point3(0, 0, 0);
    auto horizontal = Vector3f(viewport_width, 0, 0);
    auto vertical = Vector3f(0, viewport_height, 0);
    auto lower_left_corner = origin - horizontal / 2 - vertical / 2 - Vector3f (0, 0, focal_length);
>>>>>>> parent of 70be2a2 (update camera and material)

    // Render

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = image_height - 1; j >= 0; -- j)
    {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++ i)
        {
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
            Color pixel_Color(0, 0, 0);
            for (int s = 0; s < spp; s ++)
            {
                auto u = (i + randomFloat()) / (image_width - 1);
                auto v = (j + randomFloat()) / (image_height - 1);
                Ray r = cam.getRay(u, v);
                pixel_Color += castRay(r, world, max_depth);
            }
            writeColor(std::cout, pixel_Color, spp);
=======
            auto r = double(i) / (image_width - 1);
            auto g = double(j) / (image_height - 1);
            auto b = 0.25;

            int ir = static_cast<int>(255.999 * r);
            int ig = static_cast<int>(255.999 * g);
            int ib = static_cast<int>(255.999 * b);

            std::cout << ir << ' ' << ig << ' ' << ib << '\n';
>>>>>>> parent of d8f8d70 (add vector and color class)
=======
            Color pixel_color(double(i) / (image_width - 1), double(j) / (image_height - 1), 0.25);
            write_color(std::cout, pixel_color);
>>>>>>> parent of 51fe3f2 (add ray, sphere, hit class)
=======
            auto u = float(i) / (image_width - 1);
            auto v = float(j) / (image_height - 1);
            Vector3f dir = lower_left_corner + u * horizontal + v * vertical - origin;
            Ray r(origin, dir);
            Color pixel_color = castRay(r, world);
            writeColor(std::cout, pixel_color);
>>>>>>> parent of 70be2a2 (update camera and material)
        }
    }

    std::cerr << "\nDone.\n";

    return 0;
}