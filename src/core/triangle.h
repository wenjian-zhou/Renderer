#ifndef RENDERER_TRIANGLE_H
#define RENDERER_TRIANGLE_H

#include "hittable.h"
#include "vec3.h"

class triangle : public hittable
{
public:
    triangle() {}
    triangle(vec3 _v0, vec3 _v1, vec3 _v2, shared_ptr<material> m) : v0(_v0), v1(_v1), v2(_v2), mat_ptr(m)
    {
        e1 = v1 - v0;
        e2 = v2 - v0;
        normal = unit_vector(cross(e1, e2));
        area = cross(e1, e2).length() * 0.5f;
    }

    virtual bool hit(const ray &r, double t_min, double t_max, hit_record &rec) const override;
    virtual bool bounding_box(double time0, double time1, aabb &output_box) const override;

public:
    vec3 v0, v1, v2; // vertices A, B, C, counter-clockwise order
    vec3 e1, e2; // 2 edges v1-v0, v2-v0
    vec3 t0, t1, t2; // texture coords
    vec3 normal;
    double area;
    std::shared_ptr<material> mat_ptr;
};

bool triangle::hit(const ray &r, double t_min, double t_max, hit_record &rec) const
{
    vec3 edge1 = v1 - v0;
    vec3 edge2 = v2 - v0;
    vec3 pvec = cross(r.dir, edge2);
    float det = dot(edge1, pvec);
    if (det == 0 || det < 0)
        return false;

    vec3 tvec = r.orig - v0;
    rec.u = dot(tvec, pvec);
    if (rec.u < 0 || rec.u > det)
        return false;

    vec3 qvec = cross(tvec, edge1);
    rec.v = dot(r.dir, qvec);
    if (rec.v < 0 || rec.u + rec.v > det)
        return false;

    float invDet = 1 / det;

    rec.t = dot(edge2, qvec) * invDet;
    rec.u *= invDet;
    rec.v *= invDet;
    rec.normal = normal;
    rec.mat_ptr = mat_ptr;
    rec.set_face_normal(r, normal);

    return true;
}

bool triangle::bounding_box(double time0, double time1, aabb &output_box) const
{
    vec3 min = vec3(
        std::min(v0.x(), std::min(v1.x(), v2.x())),
        std::min(v0.y(), std::min(v1.y(), v2.y())),
        std::min(v0.z(), std::min(v1.z(), v2.z()))
    );

    vec3 max = vec3(
        std::max(v0.x(), std::max(v1.x(), v2.x())),
        std::max(v0.y(), std::max(v1.y(), v2.y())),
        std::max(v0.z(), std::max(v1.z(), v2.z()))
    );

    output_box = aabb(min, max);

    return true;
}

#endif