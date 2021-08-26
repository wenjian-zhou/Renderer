#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "Hittable.hpp"
#include "AABB.hpp"

#include <memory>
#include <vector>

using std::shared_ptr;
using std::make_shared;

class HittableList : public Hittable
{
public:
    HittableList() {}
    HittableList(shared_ptr<Hittable> object) { add(object); }

    void clear() { objects.clear(); }
    void add(shared_ptr<Hittable> object) { objects.push_back(object); }

    virtual bool hit(const Ray &r, float t_min, float t_max, HitRecord &rec) const override;
    virtual bool bounding_box(float time0, float time1, AABB &output_box) const override;

public:
    std::vector<shared_ptr<Hittable>> objects;
};

bool HittableList::hit(const Ray &r, float t_min, float t_max, HitRecord &rec) const 
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

bool HittableList::bounding_box(float time0, float time1, AABB &output_box) const
{
    if (objects.empty()) return false;

    AABB temp;
    bool first_box = true;

    for (const auto &object : objects)
    {
        if (!object->bounding_box(time0, time1, temp)) return false;
        output_box = first_box ? temp : surrounding_box(temp, output_box);
        first_box = true;
    }

    return true;
}

#endif