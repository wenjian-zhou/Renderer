//
// Created by Wenjian Zhou on 2021/9/5.
//

#ifndef RENDERER_AARECT_H
#define RENDERER_AARECT_H

#include "../core/object.h"

class XYRect : public Object
{
public:
    XYRect() {}

    XYRect(double _x0, double _x1, double _y0, double _y1, double _k,
            shared_ptr<Material> mat)
        : x0(_x0), x1(_x1), y0(_y0), y1(_y1), k(_k), mp(mat){};

    virtual bool hit(const Ray &r, double t_min, double t_max, HitRecord &rec) const override;

    virtual bool bounding_box(double time0, double time1, AABB &output_box) const override
    {
        // The bounding box must have non-zero width in each dimension, so pad the Z
        // dimension a small amount.
        output_box = AABB(Point3f(x0, y0, k - 0.0001), Point3f(x1, y1, k + 0.0001));
        return true;
    }
    virtual bool Intersect(const Ray &ray, HitRecord &isect) const override;

public:
    shared_ptr<Material> mp;
    double x0, x1, y0, y1, k;
};

class XZRect : public Object
{
public:
    XZRect() {}

    XZRect(double _x0, double _x1, double _z0, double _z1, double _k,
            shared_ptr<Material> mat)
        : x0(_x0), x1(_x1), z0(_z0), z1(_z1), k(_k), mp(mat){};

    virtual bool hit(const Ray &r, double t_min, double t_max, HitRecord &rec) const override;

    virtual bool bounding_box(double time0, double time1, AABB &output_box) const override
    {
        // The bounding box must have non-zero width in each dimension, so pad the Y
        // dimension a small amount.
        output_box = AABB(Point3f(x0, k - 0.0001, z0), Point3f(x1, k + 0.0001, z1));
        return true;
    }

    virtual double pdf_value(const Point3f &origin, const Vector3f &v) const override
    {
        HitRecord rec;
        if (!this->Intersect(Ray(origin, v), rec))
            return 0;

        auto area = (x1 - x0) * (z1 - z0);
        auto distance_squared = rec.t * rec.t * v.LengthSquared();
        auto cosine = fabs(Dot(v, rec.normal) / v.Length());

        //return distance_squared / (cosine * area);
        return 1.f / area;
    }

    virtual Vector3f random(const Point3f &origin) const override
    {
        auto random_point = Point3f(random_double(x0, x1), k, random_double(z0, z1));
        return random_point - origin;
    }
    virtual bool Intersect(const Ray &ray, HitRecord &isect) const override;

public:
    shared_ptr<Material> mp;
    double x0, x1, z0, z1, k;
};

class YZRect : public Object
{
public:
    YZRect() {}

    YZRect(double _y0, double _y1, double _z0, double _z1, double _k,
            shared_ptr<Material> mat)
        : y0(_y0), y1(_y1), z0(_z0), z1(_z1), k(_k), mp(mat){};

    virtual bool hit(const Ray &r, double t_min, double t_max, HitRecord &rec) const override;

    virtual bool bounding_box(double time0, double time1, AABB &output_box) const override
    {
        // The bounding box must have non-zero width in each dimension, so pad the X
        // dimension a small amount.
        output_box = AABB(Point3f(k - 0.0001, y0, z0), Point3f(k + 0.0001, y1, z1));
        return true;
    }
    virtual bool Intersect(const Ray &ray, HitRecord &isect) const override;

public:
    shared_ptr<Material> mp;
    double y0, y1, z0, z1, k;
};

bool XYRect::hit(const Ray &r, double t_min, double t_max, HitRecord &rec) const
{
    auto t = (k - r.o.z) / r.d.z;
    if (t < t_min || t > t_max)
        return false;
    auto x = r.o.x + t * r.d.x;
    auto y = r.o.y + t * r.d.y;
    if (x < x0 || x > x1 || y < y0 || y > y1)
        return false;
    rec.u = (x - x0) / (x1 - x0);
    rec.v = (y - y0) / (y1 - y0);
    rec.t = t;
    auto outward_normal = Vector3f(0, 0, 1);
    rec.set_face_normal(r, outward_normal);
    rec.mat_ptr = mp;
    rec.p = r(t);
    return true;
}

bool XYRect::Intersect(const Ray &ray, HitRecord &isect) const {
    auto t = (k - ray.o.z) / ray.d.z;
    if (t < 0.0001f || t > ray.tMax)
        return false;
    auto x = ray.o.x + t * ray.d.x;
    auto y = ray.o.y + t * ray.d.y;
    if (x < x0 || x > x1 || y < y0 || y > y1)
        return false;
    isect.u = (x - x0) / (x1 - x0);
    isect.v = (y - y0) / (y1 - y0);
    ray.tMax = t;
    isect.t = t;
    auto outward_normal = Vector3f(0, 0, 1);
    isect.set_face_normal(ray, outward_normal);
    isect.mat_ptr = mp;
    isect.p = ray(t);
    isect.wo = -ray.d;
    return true;
}

bool XZRect::hit(const Ray &r, double t_min, double t_max, HitRecord &rec) const
{
    auto t = (k - r.o.y) / r.d.y;
    if (t < t_min || t > t_max)
        return false;
    auto x = r.o.x + t * r.d.x;
    auto z = r.o.z + t * r.d.z;
    if (x < x0 || x > x1 || z < z0 || z > z1)
        return false;
    rec.u = (x - x0) / (x1 - x0);
    rec.v = (z - z0) / (z1 - z0);
    rec.t = t;
    auto outward_normal = Vector3f(0, 1, 0);
    rec.set_face_normal(r, outward_normal);
    rec.mat_ptr = mp;
    rec.p = r(t);
    return true;
}

bool XZRect::Intersect(const Ray &ray, HitRecord &isect) const {
    auto t = (k - ray.o.y) / ray.d.y;
    if (t < 0.0001f || t > ray.tMax)
        return false;
    auto x = ray.o.x + t * ray.d.x;
    auto z = ray.o.z + t * ray.d.z;
    if (x < x0 || x > x1 || z < z0 || z > z1)
        return false;
    isect.u = (x - x0) / (x1 - x0);
    isect.v = (z - z0) / (z1 - z0);
    ray.tMax = t;
    isect.t = t;
    auto outward_normal = Vector3f(0, 1, 0);
    isect.set_face_normal(ray, outward_normal);
    isect.mat_ptr = mp;
    isect.p = ray(t);
    isect.wo = -ray.d;
    return true;
}

bool YZRect::hit(const Ray &r, double t_min, double t_max, HitRecord &rec) const
{
    auto t = (k - r.o.x) / r.d.x;
    if (t < t_min || t > t_max)
        return false;
    auto y = r.o.y + t * r.d.y;
    auto z = r.o.z + t * r.d.z;
    if (y < y0 || y > y1 || z < z0 || z > z1)
        return false;
    rec.u = (y - y0) / (y1 - y0);
    rec.v = (z - z0) / (z1 - z0);
    rec.t = t;
    auto outward_normal = Vector3f(1, 0, 0);
    rec.set_face_normal(r, outward_normal);
    rec.mat_ptr = mp;
    rec.p = r(t);
    return true;
}

bool YZRect::Intersect(const Ray &ray, HitRecord &isect) const {
    auto t = (k - ray.o.x) / ray.d.x;
    if (t < 0.0001f || t > ray.tMax)
        return false;
    auto y = ray.o.y + t * ray.d.y;
    auto z = ray.o.z + t * ray.d.z;
    if (y < y0 || y > y1 || z < z0 || z > z1)
        return false;
    isect.u = (y - y0) / (y1 - y0);
    isect.v = (z - z0) / (z1 - z0);
    ray.tMax = t;
    isect.t = t;
    auto outward_normal = Vector3f(1, 0, 0);
    isect.set_face_normal(ray, outward_normal);
    isect.mat_ptr = mp;
    isect.p = ray(t);
    isect.wo = -ray.d;
    return true;
}

#endif //RENDERER_AARECT_H
