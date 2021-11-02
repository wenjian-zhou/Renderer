#ifndef MATERIAL_H
#define MATERIAL_H

#include "global.h"
#include "object.h"
#include "texture.h"
#include "onb.h"
#include "record.h"

struct HitRecord;

struct scatter_record
{
    Ray specular_Ray;
    bool is_specular;
    Spectrum attenuation;
};

Vector3f toWorld(const Vector3f &a, const Vector3f &N)
{
    Vector3f B, C;
    if (std::fabs(N.x) > std::fabs(N.y))
    {
        float invLen = 1.0f / safe_sqrt(N[0] * N[0] + N[2] * N[2]);
        C = Vector3f(N[2] * invLen, 0.0f, -N[0] * invLen);
    }
    else 
    {
        float invLen = 1.0f / safe_sqrt(N[1] * N[1] + N[2] * N[2]);
        C = Vector3f(0.0f, N[2] * invLen, -N[1] * invLen);
    }
    B = Cross(C, N);
    return a.x * B + a.y * C + a.z * N;
}

void fresnel(const Vector3f &I, const Vector3f &N, const float &ior, float &kr)
{
    float cosi = clamp(Dot(I, N), -1, 1);
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

Vector3f fresnelConductorExact(float cosThetaI, const Vector3f &eta, const Vector3f &k)
{
    float cosThetaI2 = cosThetaI * cosThetaI,
          sinThetaI2 = 1 - cosThetaI2,
          sinThetaI4 = sinThetaI2 * sinThetaI2;

    Vector3f temp1 = eta * eta - k * k - Vector3f(sinThetaI2),
         a2pb2 = (temp1 * temp1 + k * k * eta * eta * 4).safe_sqrt_vec(),
         a     = ((a2pb2 + temp1) * 0.5f).safe_sqrt_vec();

    Vector3f term1 = a2pb2 + Vector3f(cosThetaI2),
         term2 = a * (2 * cosThetaI);

    Vector3f Rs2 = (term1 - term2) / (term1 + term2);

    Vector3f term3 = a2pb2 * cosThetaI2 + Vector3f(sinThetaI4),
         term4 = term2 * sinThetaI2;

    Vector3f Rp2 = Rs2 * (term3 - term4) / (term3 + term4);

    return 0.5f * (Rp2 + Rs2);
}

class Material
{
public:
    Material() {}
    virtual Spectrum emitted(
        const Ray &r_in, const HitRecord &rec, double u, double v, const Point3f &p) const
    {
        return Spectrum(0, 0, 0);
    }

    virtual bool scatter(
        const Ray &r_in, const HitRecord &rec, scatter_record &srec) const
    {
        return false;
    }

    virtual Vector3f eval(
        const Ray &r_in, const HitRecord &rec, const Ray &scattered) const
    {
        return 0;
    }

    // return BRDF
    virtual Vector3f Eval(const Vector3f &wo, const Vector3f &wi) const
    {
        return Vector3f();
    }

    // return wi and pdf
    virtual void Sample(const Vector3f &N, Vector3f &wi, float &pdf) const {}

    // return pdf
    virtual float Pdf(const Vector3f &wo, const Vector3f &wi) const 
    {
        return 0;
    }
};

class lambertian : public Material
{
public:
    lambertian(const Spectrum &a) : albedo(make_shared<Solid_Color>(a)) {}
    lambertian(shared_ptr<Texture> a) : albedo(a) {}

    virtual bool scatter(
        const Ray &r_in, const HitRecord &rec, scatter_record &srec) const override
    {
        srec.is_specular = false;
        srec.attenuation = albedo->Value(rec.u, rec.v, rec.p);
        return true;
    }
    virtual Vector3f eval(
        const Ray &r_in, const HitRecord &rec, const Ray &scattered) const override
    {
        auto cosine = Dot(rec.normal, Normalize(scattered.direction()));
        return Vector3f(cosine < 0 ? 0 : (cosine / PI));
    }

    virtual void Sample(const Vector3f &N, Vector3f &wi, float &pdf) const override
    {
        wi = random_in_hemisphere(N);
        pdf = 0.5 * invPI;
    }

    virtual Vector3f Eval(const Vector3f &wo, const Vector3f &wi) const override
    {
        return Vector3f(invPI);
    }

public:
    shared_ptr<Texture> albedo;
};

class metal : public Material
{
public:
    metal(const Spectrum &a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}
    virtual bool scatter(
        const Ray &r_in, const HitRecord &rec, scatter_record &srec) const override
    {
        Vector3f reflected = Reflect(Normalize(r_in.direction()), rec.normal);
        srec.specular_Ray = Ray(rec.p, reflected + fuzz * random_in_unit_sphere());
        srec.attenuation = albedo;
        srec.is_specular = true;
        return true;
    }

public:
    Spectrum albedo;
    double fuzz;
};

class dielectric : public Material
{
public:
    dielectric(double index_of_refraction) : ir(index_of_refraction) {}

    virtual bool scatter(
        const Ray &r_in, const HitRecord &rec, scatter_record &srec) const override
    {
        srec.is_specular = true;
        srec.attenuation = Spectrum(1.0, 1.0, 1.0);
        double refraction_ratio = rec.front_face ? (1.0 / ir) : ir;

        Vector3f unit_direction = Normalize(r_in.direction());
        double cos_theta = fmin(Dot(-unit_direction, rec.normal), 1.0);
        double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

        bool cannot_refract = refraction_ratio * sin_theta > 1.0;
        Vector3f direction;

        if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double())
            direction = Reflect(unit_direction, rec.normal);
        else
            direction = Refract(unit_direction, rec.normal, refraction_ratio);

        srec.specular_Ray = Ray(rec.p, direction, r_in.time());
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

class diffuse_light : public Material
{
public:
    diffuse_light(std::shared_ptr<Texture> a) : emit(a) {}
    diffuse_light(Spectrum c) : emit(std::make_shared<Solid_Color>(c)) {}

    virtual bool scatter(const Ray &r_in, const HitRecord &rec, scatter_record &srec) const override
    {
        return false;
    }

    virtual Spectrum emitted(const Ray &r_in, const HitRecord &rec, double u, double v,
                          const Point3f &p) const override
    {
        if (rec.front_face)
            return emit->Value(u, v, p);
        else
            return Spectrum(0, 0, 0);
    }

public:
    std::shared_ptr<Texture> emit;
};

class isotroPIc : public Material
{
public:
    isotroPIc(Spectrum c) : albedo(make_shared<Solid_Color>(c)) {}
    isotroPIc(shared_ptr<Texture> a) : albedo(a) {}

    virtual bool scatter(
        const Ray &r_in, const HitRecord &rec, scatter_record &srec) const override
    {
        srec.specular_Ray = Ray(rec.p, random_in_unit_sphere(), r_in.time());
        srec.attenuation = albedo->Value(rec.u, rec.v, rec.p);
        return true;
    }

public:
    shared_ptr<Texture> albedo;
};

class conductor : public Material
{ // use the same sampling method and pdf as lambertian
public:
    conductor(const Spectrum &a, float alphaG, Vector3f eta, Vector3f kappa) : albedo(make_shared<Solid_Color>(a)), m_alphaG(alphaG), m_eta(eta), m_kappa(kappa) {}
    conductor(shared_ptr<Texture> a, float alphaG, Vector3f eta, Vector3f kappa) : albedo(a), m_alphaG(alphaG), m_eta(eta), m_kappa(kappa) {}

    float GGX_D(const float &theta_m) const
    {
        float cosTheta2 = pow(std::cos(theta_m), 2.0);
        float tanTheta2 = pow(std::tan(theta_m), 2.0);
        float root = m_alphaG / (cosTheta2 * (m_alphaG * m_alphaG + tanTheta2));
        float invPI = 1.0f / PI;
        return invPI * root * root;
    }

    float Smith_G(const Vector3f &wi, const Vector3f &wo, const Vector3f &wm, const HitRecord &rec) const
    {
        auto Smith1 = [&](Vector3f v, Vector3f m, const HitRecord &rec)
        {
            float cosTheta = Dot(rec.normal, v);
            float theta_v = std::acos(cosTheta);
            float tanTheta_v = std::tan(theta_v);

            if (tanTheta_v == 0.0f)
            {
                return 1.0f;
            }

            if (Dot(v, m) * Dot(v, rec.normal) <= 0)
            {
                return 0.0f;
            }

            float root = m_alphaG * tanTheta_v;

            return 2.0f / (1.0f + std::sqrt(1.0f + root * root));
        };

        return Smith1(wo, wm, rec) * Smith1(wi, wm, rec);
    }

    virtual bool scatter(
        const Ray &r_in, const HitRecord &rec, scatter_record &srec) const override
    {
        srec.is_specular = false;
        srec.attenuation = albedo->Value(rec.u, rec.v, rec.p);
        return true;
    }

    virtual Vector3f eval(
        const Ray &r_in, const HitRecord &rec, const Ray &scattered) const override
    {
        Vector3f N = rec.normal;
        Vector3f wi = Normalize(scattered.direction());
        Vector3f wo = Normalize(-r_in.direction());
        // Vector3f wm = Normalize(sampleWm(rec.normal));
        Vector3f wm = Normalize(wi + wo);

        if (Dot(wi, rec.normal) <= 0.001f) return Vector3f(0.0f);
        // std::cout << Dot(wi, rec.normal) << std::endl;
        float cosTheta_i = Dot(wi, rec.normal);
        float cosTheta_m = Dot(wm, rec.normal);
        float cosTheta_o = Dot(wo, rec.normal);
        float cosTheta_om = Dot(wo, wm);

        // std::cout << cosTheta_om << std::endl;

        float theta_m = std::acos(cosTheta_m);

        Vector3f F; 
        // fresnel(wi, rec.normal, m_ior, F);
        // fresnelShilick(cosTheta_om, m_ior, rec);
        F = fresnelConductorExact(cosTheta_om, m_eta, m_kappa);
        float D = GGX_D(theta_m);
        float G = Smith_G(wi, wo, wm, rec);

        float AbsCosTheta_o = std::abs(cosTheta_o);
        float AbsCosTheta_i = std::abs(cosTheta_i);

        float root = 1 / (4 * AbsCosTheta_i * AbsCosTheta_o);

        Vector3f kd = Vector3f(1.0f) - F;
        Spectrum diffuse = albedo->Value(rec.u, rec.v, rec.p) / PI;
        // std::cout << "F, D, G: " << F << ' ' << D << ' ' << G << std::endl;
        // return ((1 - m_metallic) * kd * diffuse + root * D * F * G) * (cosTheta_i < 0 ? 0 : cosTheta_i);
        return root * D * F * G * (cosTheta_i < 0 ? 0 : cosTheta_i);
    }

public:
    float m_alphaG;
    shared_ptr<Texture> albedo;
    Vector3f m_eta;
    Vector3f m_kappa;
};

#endif