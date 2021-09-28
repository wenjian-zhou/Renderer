#include "PTintegrator.h"

/*
color PTIntegrator::castRay(const ray &r, const color &background, const hittable &world, int depth) const
{
    hit_record rec;

    // If we've exceeded the ray bounce limit, no more light is gathered.
    if (depth <= 0)
        return color(0, 0, 0);

    // If the ray hits nothing, return the background color.
    if (!world.hit(r, 0.001, infinity, rec))
        return background;

    ray scattered;
    color attenuation;
    color emitted = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);

    if (!rec.mat_ptr->scatter(r, rec, attenuation, scattered))
        return emitted;

    return emitted + attenuation * castRay(scattered, background, world, depth - 1);
}
*/
color PTIntegrator::castRay(const ray &r, const color &background, const hittable &world, int depth) const
{
    hit_record rec;

    double RussianRoulette = 0.8;

    if (random_double(0, 1) > RussianRoulette)
    {
        return color(0, 0, 0);
    }

    if (!world.hit(r, 0.001, infinity, rec))
    {
        return background;
    }

    ray wi;
    color fr;
    color Le = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);

    if (!rec.mat_ptr->scatter(r, rec, fr, wi))
    {
        return Le;
    }

    return (Le + fr * castRay(wi, background, world, depth - 1)) / RussianRoulette;
}