#ifndef ENVMAP_H
#define ENVMAP_H

#include "hittable.h"
#include "vec3.h"
#include "onb.h"
#include "pdf.h"

class envmap : public hittable
{
public:
    envmap() {}
    envmap(point3 cen, double r, shared_ptr<material> m)
        : center(cen), radius(r), mat_ptr(m){};

    virtual bool hit(
        const ray &r, double t_min, double t_max, hit_record &rec) const override;

    virtual bool bounding_box(double time0, double time1, aabb &output_box) const override;

    virtual double pdf_value(const point3 &o, const vec3 &v) const override;

    virtual vec3 random(const point3 &o) const override;

public:
    point3 center;
    double radius;
    shared_ptr<material> mat_ptr;

private:
    static void get_envmap_uv(const point3 &p, double &u, double &v)
    {
        auto theta = acos(-p.y());
        auto phi = atan2(-p.z(), p.x()) + pi;

        u = phi / (2 * pi);
        v = 1.0f - (theta / pi);
    }
};

bool envmap::hit(const ray &r, double t_min, double t_max, hit_record &rec) const
{
    vec3 oc = r.origin() - center;
    auto a = r.direction().length_squared();
    auto half_b = dot(oc, r.direction());
    auto c = oc.length_squared() - radius * radius;

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
    rec.p = r.at(rec.t);
    vec3 inward_normal = (center - rec.p) / radius;
    rec.set_face_normal(r, inward_normal);
    get_envmap_uv(inward_normal, rec.u, rec.v);
    rec.mat_ptr = mat_ptr;

    return true;
}

bool envmap::bounding_box(double time0, double time1, aabb &output_box) const
{
    output_box = aabb(
        center - vec3(radius),
        center + vec3(radius));

    return true;
}

double envmap::pdf_value(const point3 &o, const vec3 &v) const
{
    hit_record rec;
    if (!this->hit(ray(o, v), 0.001, infinity, rec))
        return 0;

    auto cos_theta_max = sqrt(1 - radius * radius / (center - o).length_squared());
    auto solid_angle = 2 * pi * (1 - cos_theta_max);

    return 1 / solid_angle;
}

vec3 envmap::random(const point3 &o) const
{
    vec3 direction = center - o;
    auto distance_squared = direction.length_squared();
    onb uvw;
    uvw.build_from_w(direction);
    return uvw.local(random_to_sphere(radius, distance_squared));
}

#endif