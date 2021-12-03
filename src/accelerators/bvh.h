#ifndef BVH_H
#define BVH_H

#include <algorithm>

#include "../core/object.h"
#include "../core/hittable_list.h"

inline bool box_compare(const shared_ptr<Object> a, const shared_ptr<Object> b, int axis)
{
    AABB box_a;
    AABB box_b;

    if (!a->bounding_box(0, 0, box_a) || !b->bounding_box(0, 0, box_b))
        std::cerr << "No bounding box in bvh node constructor.\n";

    return (box_a.min())[axis] < (box_b.min())[axis];
}

bool box_x_compare(const shared_ptr<Object> a, const shared_ptr<Object> b)
{
    return box_compare(a, b, 0);
}

bool box_y_compare(const shared_ptr<Object> a, const shared_ptr<Object> b)
{
    return box_compare(a, b, 1);
}

bool box_z_compare(const shared_ptr<Object> a, const shared_ptr<Object> b)
{
    return box_compare(a, b, 2);
}

class BVH : public Object
{
public:

    BVH(const ObjectList &list, double time0, double time1, const MediumRecord &mediumRecord)
        : BVH(list.objects, 0, list.objects.size(), time0, time1, mediumRecord)
    {}

    BVH(const std::vector<shared_ptr<Object>> &src_objects,
            size_t start, size_t end, double time0, double time1, const MediumRecord &mediumRecord) : Object(mediumRecord)
    {
        auto objects = src_objects;

        int axis = random_int(0, 2);
        auto comparator = (axis == 0) ? box_x_compare
                        : (axis == 1) ? box_y_compare
                                      : box_z_compare;

        size_t object_span = end - start;

        if (object_span == 1)
        {
            left = right = objects[start];
        }
        else if (object_span == 2)
        {
            if (comparator(objects[start], objects[start + 1]))
            {
                left = objects[start];
                right = objects[start + 1];
            }
            else
            {
                left = objects[start + 1];
                right = objects[start];
            }
        }
        else
        {
            std::sort(objects.begin() + start, objects.begin() + end, comparator);

            auto mid = start + object_span / 2;
            left = make_shared<BVH>(objects, start, mid, time0, time1, mediumRecord);
            right = make_shared<BVH>(objects, mid, end, time0, time1, mediumRecord);
        }

        AABB box_left, box_right;

        if (!left->bounding_box(time0, time1, box_left) || !right->bounding_box(time0, time1, box_right))
            std::cerr << "No bounding box in bvh node constructor.\n";

        box = surrounding_box(box_left, box_right);
    }

    virtual bool hit(
            const Ray &r, double t_min, double t_max, HitRecord &rec) const override;

    virtual bool bounding_box(double time0, double time1, AABB &output_box) const override;

    virtual bool Intersect(const Ray &ray, HitRecord &isect) const override;

public:
    shared_ptr<Object> left;
    shared_ptr<Object> right;
    AABB box;
};

bool BVH::bounding_box(double time0, double time1, AABB &output_box) const
{
    output_box = box;
    return true;
}

bool BVH::hit(const Ray &r, double t_min, double t_max, HitRecord &rec) const
{
    if (!box.hit(r, t_min, t_max))
        return false;

    bool hit_left = left->hit(r, t_min, t_max, rec);
    bool hit_right = right->hit(r, t_min, hit_left ? rec.t : t_max, rec);

    return hit_left || hit_right;
}

bool BVH::Intersect(const Ray &ray, HitRecord &isect) const {
    if (!box.hit(ray, 0.0001, ray.tMax)) {
        return false;
    }
    
    bool hit_left = left->Intersect(ray, isect);
    bool hit_right = right->Intersect(ray, isect);
    return hit_left || hit_right;
}

#endif