#ifndef RECORD_H
#define RECORD_H

#include "global.h"

struct HitRecord
{
    Point3f p;
    Vector3f normal;
    shared_ptr<Material> mat_ptr;
    double t;
    double u, v;
    bool front_face;

    inline void set_face_normal(const Ray &r, const Vector3f &outward_normal)
    {
        front_face = Dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

#endif