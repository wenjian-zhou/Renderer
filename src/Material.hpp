#ifndef MATERIAL_H
#define MATERIAL_H

#include "global.hpp"
#include "Hittable.hpp"

struct HitRecord;

class Material
{
public:
    virtual bool scatter(const Ray &r_in, const HitRecord &rec, Color &attenuation, Ray &scattered) const = 0;
};

class Lambertian : public Material
{
public:
    Lambertian(const Color &a) : albedo(a) {}

    virtual bool scatter(const Ray &r_in, const HitRecord &rec, Color &attenuation, Ray &scattered) const override
    {
        auto scatter_direction = rec.normal + randomUnitVector();

        if (scatter_direction.nearZero())
        {
            scatter_direction = rec.normal;
        }

        scattered = Ray(rec.p, scatter_direction);
        attenuation = albedo;
        return true;
    }

public:
    Color albedo;
};

class Metal : public Material
{
public:
    Metal(const Color &a, float f) : albedo(a), fuzz(f < 1 ? f : 1) {}

    virtual bool scatter(const Ray &r_in, const HitRecord &rec, Color &attenuation, Ray &scattered) const override
    {
        Vector3f reflected = reflect(normalized(r_in.direction()), rec.normal);
        scattered = Ray(rec.p, normalized(reflected + fuzz * randomInUnitSphere()));
        attenuation = albedo;
        return (dot(scattered.direction(), rec.normal) > 0);
    }

public:
    Color albedo;
    float fuzz;
};

class Dielectric : public Material
{
public:
    Dielectric(float index_of_refraction) : ir(index_of_refraction) {}

    virtual bool scatter(const Ray &r_in, const HitRecord &rec, Color &attenuation, Ray &scattered) const override
    {
        attenuation = Color(1.0, 1.0, 1.0);
        float refraction_ratio = rec.front_face ? (1.0 / ir) : ir;

        Vector3f unit_direction = normalized(r_in.direction());
        float cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
        float sin_theta = sqrt(1.0 - cos_theta*cos_theta);

        bool cannot_refract = refraction_ratio * sin_theta > 1.0;
        Vector3f direction;

        if (cannot_refract || reflectance(cos_theta, refraction_ratio) > randomFloat())
        {
            direction = reflect(unit_direction, rec.normal);
        }
        else
        {
            direction = refract(unit_direction, rec.normal, refraction_ratio);
        }

        scattered = Ray(rec.p, direction);
        return true;
    }

public:
    float ir;

private:
    static float reflectance(float cosine, float ref_idx)
    {
        auto r0 = (1 - ref_idx) / (1 + ref_idx);
        r0 = r0 * r0;
        return r0 + (1 - r0) * pow((1 - cosine), 5);
    }
};

#endif