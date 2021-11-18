#include "triangle.h"

bool Triangle::Intersect(const Ray &ray, HitRecord *isect) const {
    Vector3f edge1 = v1 - v0;
    Vector3f edge2 = v2 - v0;
    Vector3f pvec = Cross(ray.d, edge2);
    float det = Dot(edge1, pvec);
    if (det == 0 || det < 0)
        return false;

    Vector3f tvec = ray.o - v0;
    isect->u = Dot(tvec, pvec);
    if (isect->u < 0 || isect->u > det)
        return false;

    Vector3f qvec = Cross(tvec, edge1);
    isect->v = Dot(ray.d, qvec);
    if (isect->v < 0 || isect->u + isect->v > det)
        return false;

    float invDet = 1 / det;

    isect->t = Dot(edge2, qvec) * invDet;
    isect->p = ray(isect->t);
    isect->u *= invDet;
    isect->v *= invDet;
    isect->normal = normal;
    isect->mat_ptr = mat_ptr;
    isect->wo = -ray.d;
    //std::cout << rec.normal << std::endl;
    return true;
}

bool Triangle::hit(const Ray &r, double t_min, double t_max, HitRecord &rec) const
{
    Vector3f edge1 = v1 - v0;
    Vector3f edge2 = v2 - v0;
    Vector3f pvec = Cross(r.d, edge2);
    float det = Dot(edge1, pvec);
    if (det == 0 || det < 0)
        return false;

    Vector3f tvec = r.o - v0;
    rec.u = Dot(tvec, pvec);
    if (rec.u < 0 || rec.u > det)
        return false;

    Vector3f qvec = Cross(tvec, edge1);
    rec.v = Dot(r.d, qvec);
    if (rec.v < 0 || rec.u + rec.v > det)
        return false;

    float invDet = 1 / det;

    rec.t = Dot(edge2, qvec) * invDet;
    rec.p = r(rec.t);
    rec.u *= invDet;
    rec.v *= invDet;
    rec.normal = normal;
    rec.mat_ptr = mat_ptr;
    //std::cout << rec.normal << std::endl;
    return true;
}

bool Triangle::bounding_box(double time0, double time1, AABB &output_box) const
{
    Vector3f min = Vector3f(
        std::min(v0.x, std::min(v1.x, v2.x)),
        std::min(v0.y, std::min(v1.y, v2.y)),
        std::min(v0.z, std::min(v1.z, v2.z))
    );

    Vector3f max = Vector3f(
        std::max(v0.x, std::max(v1.x, v2.x)),
        std::max(v0.y, std::max(v1.y, v2.y)),
        std::max(v0.z, std::max(v1.z, v2.z))
    );

    output_box = AABB(Vector3f(min.x - 0.0001f, min.y - 0.0001f, min.z - 0.0001f), 
                      Vector3f(max.x + 0.0001f, max.y + 0.0001f, max.z + 0.0001f));

    return true;
}