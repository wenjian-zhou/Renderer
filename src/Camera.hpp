#ifndef CAMERA_H
#define CAMERA_H

#include "global.hpp"

class Camera
{
public:
    Camera(Point3 lookfrom, Point3 lookat, Vector3f vup, float vfov, float aspect_ratio, float aperture, float focus_dist)
    {
        auto theta = degreesToRadians(vfov);
        auto h = tan(theta / 2);
        auto viewport_height = 2.0 * h;
        auto viewport_width = aspect_ratio * viewport_height;

        auto focal_length = 1.0;

        w = normalized(lookfrom - lookat);
        u = normalized(cross(vup, w));
        v = cross(w, u);

        origin = lookfrom;
        horizontal = focus_dist * viewport_width * u;
        vertical = focus_dist * viewport_height * v;
        lower_left_corner = origin - horizontal/2 - vertical/2 - focus_dist * w;

        lens_radius = aperture / 2;
    }

    Ray getRay(float s, float t) const
    {
        Vector3f rd = lens_radius * randomInUnitDisk();
        Vector3f offset = u * rd.x() + v * rd.y();
        return Ray(origin + offset, lower_left_corner + s * horizontal + t * vertical - origin - offset);
    }

private:
    Point3 origin;
    Point3 lower_left_corner;
    Vector3f horizontal;
    Vector3f vertical;
    Vector3f u, v, w;
    float lens_radius;
};

#endif