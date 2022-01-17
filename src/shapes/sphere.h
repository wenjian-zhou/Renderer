#ifndef SPHERE_H
#define SPHERE_H

#include "../core/object.h"

class Sphere : public Object
{
public:
    Sphere(Point3f cen, double r, shared_ptr<Material> m, std::shared_ptr<MediumRecord> mediumRecord = nullptr)
        : center(cen), radius(r), mat_ptr(m), Object(mediumRecord) {};

    virtual bool hit(
        const Ray &r, double t_min, double t_max, HitRecord &rec) const override;

    virtual bool bounding_box(double time0, double time1, AABB &output_box) const override;

    virtual double pdf_value(const Point3f &o, const Vector3f &v) const override;

    virtual Vector3f random(const Point3f &o) const override;

    bool Intersect(const Ray &ray, HitRecord &isect) const;

public:
    bool envmap;
    Point3f center;
    double radius;
    shared_ptr<Material> mat_ptr;

private:
    static void get_Sphere_uv(const Point3f &p, double &u, double &v)
    {
        auto theta = acos(-p.y);
        auto phi = atan2(-p.z, p.x) + PI;

        u = phi / (2 * PI);
        v = theta / PI;
    }
};

bool Sphere::Intersect(const Ray &ray, HitRecord &isect) const {
    Vector3f oc = ray.o - center;
    auto a = ray.d.LengthSquared();
    auto half_b = Dot(oc, ray.d);
    auto c = oc.LengthSquared() - radius * radius;

    auto discriminant = half_b * half_b - a * c;
    if (discriminant < 0)
        return false;
    auto sqrtd = sqrt(discriminant);

    auto root = (-half_b - sqrtd) / a;
    if (ray.tMax < root || root < 1 - ShadowEpsilon) {
        root = (-half_b + sqrtd) / a;
        if (ray.tMax < root || root < 1 - ShadowEpsilon) {
            return false;
        }
    }

    ray.tMax = root;
    isect.t = root;
    isect.p = ray(isect.t);
    isect.normal = (isect.p - center) / radius;
    isect.mat_ptr = mat_ptr;
    isect.wo = -ray.d;
    isect.mediumRecord = *mediumRecord;

    return true;
}

bool Sphere::hit(const Ray &r, double t_min, double t_max, HitRecord &rec) const
{
    Vector3f oc = r.o - center;
    auto a = r.d.LengthSquared();
    auto half_b = Dot(oc, r.d);
    auto c = oc.LengthSquared() - radius * radius;

    auto discriminant = half_b * half_b - a * c;
    if (discriminant < 0)
        return false;
    auto sqrtd = sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    auto root = (-half_b - sqrtd) / a;
    if (root < t_min || t_max < root)
    {
        root = (-half_b + sqrtd) / a;
        if (root < t_min || t_max < root)
            return false;
    }

    rec.t = root;
    rec.p = r(rec.t);
    Vector3f outward_normal = (rec.p - center) / radius;
    rec.set_face_normal(r, outward_normal);
    get_Sphere_uv(outward_normal, rec.u, rec.v);
    rec.mat_ptr = mat_ptr;

    return true;
}

bool Sphere::bounding_box(double time0, double time1, AABB &output_box) const
{
    output_box = AABB(
        center - Vector3f(radius),
        center + Vector3f(radius));

    return true;
}

double Sphere::pdf_value(const Point3f &o, const Vector3f &v) const
{
    HitRecord rec;
    if (!this->hit(Ray(o, v), 0.001, Infinity, rec))
        return 0;

    auto cos_theta_max = sqrt(1 - radius * radius / (center - o).LengthSquared());
    auto solid_angle = 2 * PI * (1 - cos_theta_max);

    return 1 / solid_angle;
}

Vector3f Sphere::random(const Point3f &o) const
{
    return Vector3f();
}

#endif