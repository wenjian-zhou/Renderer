#ifndef BOUND_H
#define BOUND_H

#include "vector.h"
#include "ray.h"

class Bounds3f
{
public:
    Bounds3f()
    {
        float minNum = std::numeric_limits<float>::lowest();
        float maxNum = std::numeric_limits<float>::max();
        pMin = Vector3f(maxNum);
        pMax = Vector3f(minNum);
    }
    Bounds3f(const Vector3f &v) : pMin(v), pMax(v) {}
    Bounds3f(const Vector3f &p1, const Vector3f &p2)
    {
        pMin = Vector3f(std::min(p1.x, p2.x), std::min(p1.y, p2.y), std::min(p1.z, p2.z));
        pMax = Vector3f(std::max(p1.x, p2.x), std::max(p1.y, p2.y), std::max(p1.z, p2.z));
    }
    
    Vector3f Diagonal() const { return pMax - pMin; }
    int MaxExtent() const
    {
        Vector3f d = Diagonal();
        if (d.x > d.y && d.x > d.z)
            return 0;
        else if (d.y > d.z)
            return 1;
        return 2;
    }
    float SurfaceArea() const
    {
        Vector3f d = Diagonal();
        return 2 * (d.x * d.y + d.x * d.z + d.y * d.z);
    }
    Vector3f Centroid() { return pMin * 0.5 + pMax * 0.5; }
    inline const Vector3f &operator [] (int i) const
    {
        return (i == 0) ? pMin : pMax;
    }
    bool IntersectP(const Ray &ray, float *hitt0, float *hitt1) const;
    inline bool IntersectP(const Ray &ray, const Vector3f &invDir, const int dirIsNeg[3]) const;

public:
    Vector3f pMin, pMax;
};

inline Bounds3f Union(const Bounds3f &b1, const Bounds3f &b2)
{
    Bounds3f res;
    res.pMin = Vector3f::Min(b1.pMin, b2.pMin);
    res.pMax = Vector3f::Max(b1.pMax, b2.pMax);
    return res;
}

inline bool Bounds3f::IntersectP(const Ray &ray, float *hitt0, float *hitt1) const
{
    auto invDir = Vector3f(1.f / ray.direction.x, 1.f / ray.direction.y, 1.f / ray.direction.z);
    auto tMin = (pMin - ray.origin) * invDir;
    auto tMax = (pMax - ray.origin) * invDir;

    Vector3f t_min(
        std::min(tMin.x, tMax.x),
        std::min(tMin.y, tMax.y),
        std::min(tMin.z, tMax.z)
    );

    Vector3f t_max(
        std::max(tMin.x, tMax.x),
        std::max(tMin.y, tMax.y),
        std::max(tMin.z, tMax.z)
    );

    auto t_enter = std::max(t_min.x, std::max(t_min.y, t_min.z));
    auto t_exit  = std::min(t_max.x, std::min(t_max.y, t_max.z));

    if (t_enter <= t_exit && t_exit >= 0)
    {
        *hitt0 = t_enter;
        *hitt1 = t_exit;
        return true;
    }

    return false;
}

inline bool Bounds3f::IntersectP(const Ray &ray, const Vector3f &invDir, const int dirIsNeg[3]) const
{
    
}

#endif