#ifndef CAMERA_H
#define CAMERA_H

#include "global.h"

#include "transform.h"

class Camera {
public:
    Camera() {}
    

    Transform screenToWorld, cameraToWorld;
    float fov;

};


class camera
{
public:
    camera(
        Point3f lookfrom,
        Point3f lookat,
        Vector3f vup,
        double vfov, // vertical field-of-view in degrees
        double aspect_ratio,
        double aperture,
        double focus_dist,
        double _time0 = 0,
        double _time1 = 0)
    {
        auto theta = degrees_to_radians(vfov);
        auto h = tan(theta / 2);
        auto viewport_height = 2.0 * h;
        auto viewport_width = aspect_ratio * viewport_height;

        w = Normalize(lookfrom - lookat);
        u = Normalize(Cross(vup, w));
        v = Cross(w, u);

        origin = lookfrom;
        horizontal = focus_dist * viewport_width * u;
        vertical = focus_dist * viewport_height * v;
        lower_left_corner = origin - horizontal / 2 - vertical / 2 - focus_dist * w;

        lens_radius = aperture / 2;
        time0 = _time0;
        time1 = _time1;
    }

    Ray get_Ray(double s, double t) const
    {
        Vector3f rd = lens_radius * random_in_unit_disk();
        Vector3f offset = u * rd.x + v * rd.y;

        return Ray(
            origin + offset,
            lower_left_corner + s * horizontal + t * vertical - origin - offset,
            random_double(time0, time1));
    }

private:
    Point3f origin;
    Point3f lower_left_corner;
    Vector3f horizontal;
    Vector3f vertical;
    Vector3f u, v, w;
    double lens_radius;
    double time0, time1; // shutter open/close times

};
#endif