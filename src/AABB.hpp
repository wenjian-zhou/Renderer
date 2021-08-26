#ifndef AABB_H
#define AABB_H

#include "global.hpp"

class AABB
{
public:
    AABB() {}
    AABB(const Point3 &a, const Point3 &b)
    {
        minimum = a;
        maximum = b;
    }

    Point3 min() const { return minimum; }
    Point3 max() const { return maximum; }

    bool hit(const Ray &r, float t_min, float t_max) const
    {
        for (int a = 0; a < 3; a ++)
        {
            auto t0 = fmin((minimum[a] - r.origin()[a]) / r.direction()[a],
                               (maximum[a] - r.origin()[a]) / r.direction()[a]);
            auto t1 = fmax((minimum[a] - r.origin()[a]) / r.direction()[a],
                               (maximum[a] - r.origin()[a]) / r.direction()[a]);
            t_min = fmax(t0, t_min);
            t_max = fmin(t1, t_max);
            if (t_max <= t_min)
                return false;
        }
        return true;
    }

public:
    Point3 minimum;
    Point3 maximum;
};

AABB surrounding_box(AABB box0, AABB box1)
{
    Point3 small(fmin(box0.min().x(), box1.min().x()),
                 fmin(box0.min().y(), box1.min().y()),
                 fmin(box0.min().z(), box1.min().z()));

    Point3 big(fmin(box0.max().x(), box1.max().x()),
                 fmin(box0.max().y(), box1.max().y()),
                 fmin(box0.max().z(), box1.max().z()));

    return AABB(small, big);
}

#endif