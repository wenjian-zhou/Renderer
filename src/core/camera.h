#ifndef CAMERA_H
#define CAMERA_H

#include "transform.h"
#include "sampler.h"

/*
pinhole camera
*/

class Camera {
public:
    Camera() {}
    Camera(const Transform &c2w, const float &fov, const std::shared_ptr<Medium> medium)
        : cameraToWorld(c2w), fov(fov), medium(medium) {}

    void Setup(const uint32_t& w, const uint32_t& h) {
        Transform w2s = Scale(0.5f * w, 0.5f * h, 1.f) * Translate(1.f, 1.f, 0.f) *
            Perspective(fov, w, h) * Inverse(cameraToWorld);
        screenToWorld = Inverse(w2s);
    }
    virtual void GenerateRay(const Point2f &pos, Sampler &sampler, Ray *ray) const {
        Vector3f p = screenToWorld.TransformPoint(Vector3f(pos + sampler.Next2D(), -0.1f));
        Vector3f o = cameraToWorld.TransformPoint(Vector3f(0, 0, 0));
        *ray = Ray(o, Normalize(p - o));
    }
    
public:
    Transform screenToWorld, cameraToWorld;
    float fov;
    std::shared_ptr<Medium> medium;
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

        cameraWorldDirection = Normalize(lookat - lookfrom);
        focalDistance = focus_dist;
    }

    Ray get_Ray(double s, double t) const
    {
        Vector3f rd = lens_radius * random_in_unit_disk();
        Vector3f offset = u * rd.x + v * rd.y;

        return Ray(
            origin + offset,
            lower_left_corner + s * horizontal + t * vertical - origin - offset);
    }

    virtual Spectrum We(const Ray &ray, Point2f *pRaster2 = nullptr) const 
    {
        return Spectrum(0.f);
    }

private:
    Vector3f cameraWorldDirection;
    Point3f origin;
    Point3f lower_left_corner;
    Vector3f horizontal;
    Vector3f vertical;
    Vector3f u, v, w;
    double focalDistance;
    double lens_radius;
    double time0, time1; // shutter open/close times

};

#endif