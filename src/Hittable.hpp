#ifndef HITTABLE_H
#define HITTABLE_H

<<<<<<< HEAD
#include "global.hpp"

#include "AABB.hpp"

class Material;
=======
#include "Ray.hpp"
>>>>>>> parent of 70be2a2 (update camera and material)

struct HitRecord
{
    Point3 p;
    Vector3f normal;
    float t;
    bool front_face;

    inline void setFaceNormal(const Ray &r, const Vector3f &outward_normal)
    {
        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class Hittable
{
public:
    virtual bool hit(const Ray &r, float t_min, float t_max, HitRecord &rec) const = 0;
    virtual bool bounding_box(float time0, float time1, AABB &output_box) const = 0;
};

#endif