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

void fresnel(const vec3 &I, const vec3 &N, const float &ior, float &kr)
{
    float cosi = clamp(dot(I, N), -1, 1);
    float etai = 1, etat = ior;
    if (cosi > 0)
    {
        std::swap(etai, etat);
    }
    // Compute sini using Snell's law
    float sint = etai / etat * sqrtf(std::max(0.f, 1 - cosi * cosi));
    // Total internal reflection
    if (sint >= 1)
    {
        kr = 1;
    }
    else
    {
        float cost = sqrtf(std::max(0.f, 1 - sint * sint));
        cosi = fabsf(cosi);
        float Rs = ((etat * cosi) - (etai * cost)) / ((etat * cosi) + (etai * cost));
        float Rp = ((etai * cosi) - (etat * cost)) / ((etai * cosi) + (etat * cost));
        kr = (Rs * Rs + Rp * Rp) / 2;
    }
    // As a consequence of the conservation of energy, transmittance is given by:
    // kt = 1 - kr;
}

vec3 fresnelConductorExact(float cosThetaI, const vec3 &eta, const vec3 &k)
{
    float cosThetaI2 = cosThetaI * cosThetaI,
          sinThetaI2 = 1 - cosThetaI2,
          sinThetaI4 = sinThetaI2 * sinThetaI2;

    vec3 temp1 = eta * eta - k * k - vec3(sinThetaI2),
         a2pb2 = (temp1 * temp1 + k * k * eta * eta * 4).safe_sqrt_vec(),
         a     = ((a2pb2 + temp1) * 0.5f).safe_sqrt_vec();

    vec3 term1 = a2pb2 + vec3(cosThetaI2),
         term2 = a * (2 * cosThetaI);

    vec3 Rs2 = (term1 - term2) / (term1 + term2);

    vec3 term3 = a2pb2 * cosThetaI2 + vec3(sinThetaI4),
         term4 = term2 * sinThetaI2;

    vec3 Rp2 = Rs2 * (term3 - term4) / (term3 + term4);

    return 0.5f * (Rp2 + Rs2);
}

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
        return vec3(cosine < 0 ? 0 : (cosine / pi));
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

class conductor : public material
{ // use the same sampling method and pdf as lambertian
public:
    conductor(const color &a, float alphaG, vec3 eta, vec3 kappa) : albedo(make_shared<solid_color>(a)), m_alphaG(alphaG), m_eta(eta), m_kappa(kappa) {}
    conductor(shared_ptr<texture> a, float alphaG, vec3 eta, vec3 kappa) : albedo(a), m_alphaG(alphaG), m_eta(eta), m_kappa(kappa) {}

    float GGX_D(const float &theta_m) const
    {
        float cosTheta2 = pow(std::cos(theta_m), 2.0);
        float tanTheta2 = pow(std::tan(theta_m), 2.0);
        float root = m_alphaG / (cosTheta2 * (m_alphaG * m_alphaG + tanTheta2));
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

            return 2.0f / (1.0f + std::sqrt(1.0f + root * root));
        };

        return Smith1(wo, wm, rec) * Smith1(wi, wm, rec);
    }

    virtual bool scatter(
        const ray &r_in, const hit_record &rec, scatter_record &srec) const override
    {
        srec.is_specular = false;
        srec.attenuation = albedo->value(rec.u, rec.v, rec.p);
        srec.pdf_ptr = std::make_shared<GGX_pdf>(unit_vector(-r_in.direction()), rec.normal, m_alphaG); //new cosine_pdf(rec.normal);
        return true;
    }

    virtual vec3 eval(
        const ray &r_in, const hit_record &rec, const ray &scattered) const override
    {
        vec3 N = rec.normal;
        vec3 wi = unit_vector(scattered.direction());
        vec3 wo = unit_vector(-r_in.direction());
        // vec3 wm = unit_vector(sampleWm(rec.normal));
        vec3 wm = unit_vector(wi + wo);

        if (dot(wi, rec.normal) <= 0.001f) return vec3(0.0f);
        // std::cout << dot(wi, rec.normal) << std::endl;
        float cosTheta_i = dot(wi, rec.normal);
        float cosTheta_m = dot(wm, rec.normal);
        float cosTheta_o = dot(wo, rec.normal);
        float cosTheta_om = dot(wo, wm);

        // std::cout << cosTheta_om << std::endl;

        float theta_m = std::acos(cosTheta_m);

        vec3 F; 
        // fresnel(wi, rec.normal, m_ior, F);
        // fresnelShilick(cosTheta_om, m_ior, rec);
        F = fresnelConductorExact(cosTheta_om, m_eta, m_kappa);
        float D = GGX_D(theta_m);
        float G = Smith_G(wi, wo, wm, rec);

        float AbsCosTheta_o = std::abs(cosTheta_o);
        float AbsCosTheta_i = std::abs(cosTheta_i);

        float root = 1 / (4 * AbsCosTheta_i * AbsCosTheta_o);

        vec3 kd = vec3(1.0f) - F;
        vec3 diffuse = albedo->value(rec.u, rec.v, rec.p) / pi;
        // std::cout << "F, D, G: " << F << ' ' << D << ' ' << G << std::endl;
        // return ((1 - m_metallic) * kd * diffuse + root * D * F * G) * (cosTheta_i < 0 ? 0 : cosTheta_i);
        return root * D * F * G * (cosTheta_i < 0 ? 0 : cosTheta_i);
    }

public:
    float m_alphaG;
    shared_ptr<texture> albedo;
    vec3 m_eta;
    vec3 m_kappa;
};

#endif