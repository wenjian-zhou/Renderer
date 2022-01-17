//
// Created by Wenjian Zhou on 2021/9/5.
//

#ifndef RENDERER_BOX_H
#define RENDERER_BOX_H

#include "aarect.h"
#include "hittable_list.h"

class box : public Object
{
public:
    box() {}
    box(const Point3f& p0, const Point3f& p1, shared_ptr<Material> ptr);

    virtual bool hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const override;

    virtual bool bounding_box(double time0, double time1, AABB& output_box) const override
    {
        output_box = AABB(box_min, box_max);
        return true;
    }

public:
    Point3f box_min;
    Point3f box_max;
    ObjectList sides;
};

box::box(const Point3f& p0, const Point3f& p1, shared_ptr<Material> ptr)
{
    box_min = p0;
    box_max = p1;

    sides.add(make_shared<XYRect>(p0.x, p1.x, p0.y, p1.y, p1.z, ptr));
    sides.add(make_shared<XYRect>(p0.x, p1.x, p0.y, p1.y, p0.z, ptr));

    sides.add(make_shared<XZRect>(p0.x, p1.x, p0.z, p1.z, p1.y, ptr));
    sides.add(make_shared<XZRect>(p0.x, p1.x, p0.z, p1.z, p0.y, ptr));

    sides.add(make_shared<YZRect>(p0.y, p1.y, p0.z, p1.z, p1.x, ptr));
    sides.add(make_shared<YZRect>(p0.y, p1.y, p0.z, p1.z, p0.x, ptr));
}

bool box::hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const
{
    return sides.hit(r, t_min, t_max, rec);
}

#endif //RENDERER_BOX_H
