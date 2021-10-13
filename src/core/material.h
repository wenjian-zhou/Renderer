#ifndef MATERIAL_H
#define MATERIAL_H

#include "global.h"
#include "hittable.h"
#include "texture.h"
#include "onb.h"
#include "pdf.h"

struct hit_record;

struct scatter_record
{
    ray specular_ray;
    bool is_specular;
    color attenuation;
    shared_ptr<pdf> pdf_ptr;
};

class material
{
public:
    material() {}
    virtual color emitted(
        const ray &r_in, const hit_record &rec, double u, double v, const point3 &p) const
    {
        return color(0, 0, 0);
    }

    virtual bool scatter(
        const ray &r_in, const hit_record &rec, scatter_record &srec) const
    {
        return false;
    }

    virtual vec3 eval(
        const ray &r_in, const hit_record &rec, const ray &scattered) const
    {
        return 0;
    }
};

class lambertian : public material
{
public:
    lambertian(const color &a) : albedo(make_shared<solid_color>(a)) {}
    lambertian(shared_ptr<texture> a) : albedo(a) {}

    virtual bool scatter(
        const ray &r_in, const hit_record &rec, scatter_record &srec) const override
    {
        srec.is_specular = false;
        srec.attenuation = albedo->value(rec.u, rec.v, rec.p);
        srec.pdf_ptr = std::make_shared<cosine_pdf>(rec.normal); //new cosine_pdf(rec.normal);
        return true;
    }
    virtual vec3 eval(
        const ray &r_in, const hit_record &rec, const ray &scattered) const override
    {
        auto cosine = dot(rec.normal, unit_vector(scattered.direction()));
        return vec3(cosine < 0 ? 0 : cosine / pi);
    }

public:
    shared_ptr<texture> albedo;
};

class metal : public material
{
public:
    metal(const color &a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}
    virtual bool scatter(
        const ray &r_in, const hit_record &rec, scatter_record &srec) const override
    {
        vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
        srec.specular_ray = ray(rec.p, reflected + fuzz * random_in_unit_sphere());
        srec.attenuation = albedo;
        srec.is_specular = true;
        srec.pdf_ptr = 0;
        return true;
    }

public:
    color albedo;
    double fuzz;
};

class dielectric : public material
{
public:
    dielectric(double index_of_refraction) : ir(index_of_refraction) {}

    virtual bool scatter(
        const ray &r_in, const hit_record &rec, scatter_record &srec) const override
    {
        srec.is_specular = true;
        srec.pdf_ptr = nullptr;
        srec.attenuation = color(1.0, 1.0, 1.0);
        double refraction_ratio = rec.front_face ? (1.0 / ir) : ir;

        vec3 unit_direction = unit_vector(r_in.direction());
        double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
        double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

        bool cannot_refract = refraction_ratio * sin_theta > 1.0;
        vec3 direction;

        if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double())
            direction = reflect(unit_direction, rec.normal);
        else
            direction = refract(unit_direction, rec.normal, refraction_ratio);

        srec.specular_ray = ray(rec.p, direction, r_in.time());
        return true;
    }

public:
    double ir; // Index of Refraction

private:
    static double reflectance(double cosine, double ref_idx)
    {
        // Use Schlick's approximation for reflectance.
        auto r0 = (1 - ref_idx) / (1 + ref_idx);
        r0 = r0 * r0;
        return r0 + (1 - r0) * pow((1 - cosine), 5);
    }
};

class diffuse_light : public material
{
public:
    diffuse_light(std::shared_ptr<texture> a) : emit(a) {}
    diffuse_light(color c) : emit(std::make_shared<solid_color>(c)) {}

    virtual bool scatter(const ray &r_in, const hit_record &rec, scatter_record &srec) const override
    {
        return false;
    }

    virtual color emitted(const ray &r_in, const hit_record &rec, double u, double v,
                          const point3 &p) const override
    {
        if (rec.front_face)
            return emit->value(u, v, p);
        else
            return color(0, 0, 0);
    }

public:
    std::shared_ptr<texture> emit;
};

class isotropic : public material
{
public:
    isotropic(color c) : albedo(make_shared<solid_color>(c)) {}
    isotropic(shared_ptr<texture> a) : albedo(a) {}

    virtual bool scatter(
        const ray &r_in, const hit_record &rec, scatter_record &srec) const override
    {
        srec.specular_ray = ray(rec.p, random_in_unit_sphere(), r_in.time());
        srec.attenuation = albedo->value(rec.u, rec.v, rec.p);
        return true;
    }

public:
    shared_ptr<texture> albedo;
};

class GGX : public material
{   // use the same sampling method and pdf as lambertian
public:
    GGX(const color &a, float alphaG, vec3 R0) : albedo(make_shared<solid_color>(a)), m_alphaG(alphaG), m_R0(R0) {}
    GGX(shared_ptr<texture> a, float alphaG, vec3 R0) : albedo(a), m_alphaG(alphaG), m_R0(R0) {}

    /*
    vec3 sampleWm(const vec3 &normal) const
    {
        float x1 = random_double(0, 1.0f - 1e-5), x2 = random_double(0, 1);
        float theta = std::atan(m_alphaG * std::sqrt(x1) / std::sqrt(1 - x1));
        float kesai = 2 * pi * x2;

        float z = std::cos(theta);
        float x = std::sin(theta) * std::cos(kesai);
        float y = std::sin(theta) * std::sin(kesai);

        return vec3(x, y, z);
    }
    */

    vec3 fresnelShilick(float cosTheta, vec3 R0) const
    {
        return R0 + (1.0 - R0) * pow(1.0 - cosTheta, 5.0);
    }

    float GGX_D(const float &theta_m) const
    {
        float cosTheta2 = pow(std::cos(theta_m), 2.0);
        float tanTheta2 = pow(std::tan(theta_m), 2.0);
        float root = m_alphaG / cosTheta2 * (std::pow(m_alphaG, 2.0) * tanTheta2);
        float invPi = 1.0f / pi;
        return invPi * root * root;
    }

    float Smith_G(const vec3 &wi, const vec3 &wo, const vec3 &wm, const hit_record &rec) const
    {
        auto Smith1 = [&](vec3 v, vec3 m, const hit_record &rec)
        {
            float cosTheta = dot(rec.normal, v);
            float theta_v = std::acos(cosTheta);
            float tanTheta_v = std::tan(theta_v);

            if (tanTheta_v == 0.0f)
            {
                return 1.0f;
            }

            if (dot(v, m) * dot(v, rec.normal) <= 0)
            {
                return 0.0f;
            }

            float root = m_alphaG * tanTheta_v;

            return 2.0f / (1.0f + std::sqrt(1 + root * root));
        };

        return Smith1(wi, wm, rec) * Smith1(wo, wm, rec);
    }

    virtual bool scatter(
        const ray &r_in, const hit_record &rec, scatter_record &srec) const override
    {
        srec.is_specular = false;
        srec.attenuation = albedo->value(rec.u, rec.v, rec.p);
        srec.pdf_ptr = std::make_shared<cosine_pdf>(rec.normal); //new cosine_pdf(rec.normal);
        return true;
    }

    virtual vec3 eval(
        const ray &r_in, const hit_record &rec, const ray &scattered) const override
    {
        vec3 wi = unit_vector(scattered.direction());
        vec3 wo = unit_vector(-r_in.direction());
        vec3 wm = wi + wo / (wi + wo).length();

        float cosTheta_i = dot(wi, rec.normal);
        float cosTheta_m = dot(wm, rec.normal);
        float cosTheta_o = dot(wo, rec.normal);

        vec3 F = fresnelShilick(cosTheta_o, m_R0);
        float D = GGX_D(cosTheta_m);
        float G = Smith_G(wi, wo, wm, rec);

        float AbsCosTheta_o = std::abs(cosTheta_o);
        float AbsCosTheta_i = std::abs(cosTheta_i);

        float root = 1 / (4 * AbsCosTheta_i * AbsCosTheta_o);
        return root * D * F * G * cosTheta_i;
    }

public:
    float m_alphaG;
    shared_ptr<texture> albedo;
    vec3 m_R0;
};

#endif