#ifndef HITTABLE_H
#define HITTABLE_H

#include "global.h"
#include "ray.h"
#include "../accelerators/aabb.h"
#include "record.h"

class material;

class Object
{
public:
    virtual bool hit(const Ray &r, double t_min, double t_max, HitRecord &rec) const = 0;
    virtual bool bounding_box(double time0, double time1, aabb &output_box) const = 0;
    virtual double pdf_value(const Point3f &o, const Vector3f &v) const
    {
        return 0.0;
    }

    virtual Vector3f random(const Vector3f &o) const
    {
        return Vector3f(1, 0, 0);
    }
};

class translate : public Object
{
public:
    translate(shared_ptr<Object> p, const Vector3f &displacement)
        : ptr(p), offset(displacement) {}

    virtual bool hit(
        const Ray &r, double t_min, double t_max, HitRecord &rec) const override;

    virtual bool bounding_box(double time0, double time1, aabb &output_box) const override;

public:
    shared_ptr<Object> ptr;
    Vector3f offset;
};

class rotate_y : public Object
{
public:
    rotate_y(shared_ptr<Object> p, double angle);

    virtual bool hit(
        const Ray &r, double t_min, double t_max, HitRecord &rec) const override;

    virtual bool bounding_box(double time0, double time1, aabb &output_box) const override
    {
        output_box = bbox;
        return hasbox;
    }

public:
    shared_ptr<Object> ptr;
    double sin_theta;
    double cos_theta;
    bool hasbox;
    aabb bbox;
};

rotate_y::rotate_y(shared_ptr<Object> p, double angle) : ptr(p)
{
    auto radians = degrees_to_radians(angle);
    sin_theta = sin(radians);
    cos_theta = cos(radians);
    hasbox = ptr->bounding_box(0, 1, bbox);

    Point3f min(INF, INF, INF);
    Point3f max(-INF, -INF, -INF);

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            for (int k = 0; k < 2; k++)
            {
                auto x = i * bbox.max().x + (1 - i) * bbox.min().x;
                auto y = j * bbox.max().y + (1 - j) * bbox.min().y;
                auto z = k * bbox.max().z + (1 - k) * bbox.min().z;

                auto newx = cos_theta * x + sin_theta * z;
                auto newz = -sin_theta * x + cos_theta * z;

                Vector3f tester(newx, y, newz);

                for (int c = 0; c < 3; c++)
                {
                    min[c] = fmin(min[c], tester[c]);
                    max[c] = fmax(max[c], tester[c]);
                }
            }
        }
    }

    bbox = aabb(min, max);
}

bool translate::hit(const Ray &r, double t_min, double t_max, HitRecord &rec) const
{
    Ray moved_r(r.origin() - offset, r.direction(), r.time());
    if (!ptr->hit(moved_r, t_min, t_max, rec))
        return false;

    rec.p += offset;
    rec.set_face_normal(moved_r, rec.normal);

    return true;
}

bool translate::bounding_box(double time0, double time1, aabb &output_box) const
{
    if (!ptr->bounding_box(time0, time1, output_box))
        return false;

    output_box = aabb(
        output_box.min() + offset,
        output_box.max() + offset);

    return true;
}

bool rotate_y::hit(const Ray &r, double t_min, double t_max, HitRecord &rec) const
{
    auto origin = r.origin();
    auto direction = r.direction();

    origin[0] = cos_theta * r.origin()[0] - sin_theta * r.origin()[2];
    origin[2] = sin_theta * r.origin()[0] + cos_theta * r.origin()[2];

    direction[0] = cos_theta * r.direction()[0] - sin_theta * r.direction()[2];
    direction[2] = sin_theta * r.direction()[0] + cos_theta * r.direction()[2];

    Ray rotated_r(origin, direction, r.time());

    if (!ptr->hit(rotated_r, t_min, t_max, rec))
        return false;

    auto p = rec.p;
    auto normal = rec.normal;

    p[0] = cos_theta * rec.p[0] + sin_theta * rec.p[2];
    p[2] = -sin_theta * rec.p[0] + cos_theta * rec.p[2];

    normal[0] = cos_theta * rec.normal[0] + sin_theta * rec.normal[2];
    normal[2] = -sin_theta * rec.normal[0] + cos_theta * rec.normal[2];

    rec.p = p;
    rec.set_face_normal(rotated_r, normal);

    return true;
}

class flip_face : public Object
{
public:
    flip_face(shared_ptr<Object> p) : ptr(p) {}

    virtual bool hit(
        const Ray &r, double t_min, double t_max, HitRecord &rec) const override
    {

        if (!ptr->hit(r, t_min, t_max, rec))
            return false;

        rec.front_face = !rec.front_face;
        return true;
    }

    virtual bool bounding_box(double time0, double time1, aabb &output_box) const override
    {
        return ptr->bounding_box(time0, time1, output_box);
    }

public:
    shared_ptr<Object> ptr;
};

#endif