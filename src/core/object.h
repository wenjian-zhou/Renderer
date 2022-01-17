#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"
#include "vector.h"
#include "../accelerators/aabb.h"
#include "record.h"

class Object
{
public:
    Object(std::shared_ptr<MediumRecord> mediumRecord = nullptr) : mediumRecord(mediumRecord) {}
    virtual bool hit(const Ray &r, double t_min, double t_max, HitRecord &rec) const = 0;
    virtual bool bounding_box(double time0, double time1, AABB &output_box) const = 0;
    virtual double pdf_value(const Point3f &o, const Vector3f &v) const
    {
        return 0.0;
    }

    virtual Vector3f random(const Vector3f &o) const
    {
        return Vector3f(1, 0, 0);
    }

    virtual bool Intersect(const Ray &ray, HitRecord &isect) const = 0;

public:
    float area = 0;
    std::shared_ptr<MediumRecord> mediumRecord;
};

#endif