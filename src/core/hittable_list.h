#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "Object.h"
#include "AABB.h"

#include <memory>
#include <vector>

using std::make_shared;
using std::shared_ptr;

class ObjectList : public Object
{
public:
    ObjectList() {}
    ObjectList(shared_ptr<Object> object) { add(object); }

    void clear() { objects.clear(); }
    void add(shared_ptr<Object> object) { objects.push_back(object); }

    virtual bool hit(
        const Ray &r, double t_min, double t_max, HitRecord &rec) const override;

    virtual bool bounding_box(
        double time0, double time1, AABB &output_box) const override;

    virtual double pdf_value(const Point3f &o, const Vector3f &v) const override;

    virtual Vector3f random(const Vector3f &o) const override;

public:
    std::vector<shared_ptr<Object>> objects;
};

bool ObjectList::hit(const Ray &r, double t_min, double t_max, HitRecord &rec) const
{
    HitRecord temp_rec;
    bool hit_anything = false;
    auto closest_so_far = t_max;

    for (const auto &object : objects)
    {
        if (object->hit(r, t_min, closest_so_far, temp_rec))
        {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }

    return hit_anything;
}

bool ObjectList::bounding_box(double time0, double time1, AABB &output_box) const
{
    if (objects.empty())
        return false;

    AABB temp_box;
    bool first_box = true;

    for (const auto &object : objects)
    {
        if (!object->bounding_box(time0, time1, temp_box))
            return false;
        output_box = first_box ? temp_box : surrounding_box(output_box, temp_box);
        first_box = false;
    }

    return true;
}

double ObjectList::pdf_value(const Point3f &o, const Vector3f &v) const
{
    auto weight = 1.0 / objects.size();
    auto sum = 0.0;

    for (const auto &object : objects)
        sum += weight * object->pdf_value(o, v);

    return sum;
}

Vector3f ObjectList::random(const Vector3f &o) const
{
    auto int_size = static_cast<int>(objects.size());
    return objects[random_int(0, int_size - 1)]->random(o);
}

#endif