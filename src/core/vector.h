#ifndef VEC3_H
#define VEC3_H

#include "global.h"

#include <assert.h>
#include <cmath>
#include <iostream>

using std::sqrt;

class vec3
{
public:
    vec3() { x = y = z = 0; }
    vec3(float xx) : x(xx), y(xx), z(xx) {}
    vec3(float xx, float yy, float zz) : x(xx), y(yy), z(zz) {}

    vec3 operator-() const { return vec3(-x, -y, -z); }
    float operator[](int i) const { return (i == 0) ? x : (i == 1) ? y : z; }
    float &operator[](int i) { return (i == 0) ? x : (i == 1) ? y : z; }

    vec3 &operator+=(const vec3 &v)
    {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }

    vec3 &operator*=(const double t)
    {
        x *= t;
        y *= t;
        z *= t;
        return *this;
    }

    vec3 &operator/=(const double t)
    {
        return *this *= 1 / t;
    }

    double length() const
    {
        return sqrt(length_squared());
    }

    double length_squared() const
    {
        return x * x + y * y + z * z;
    }

    inline static vec3 random()
    {
        return vec3(random_double(), random_double(), random_double());
    }

    inline static vec3 random(double min, double max)
    {
        return vec3(random_double(min, max), random_double(min, max), random_double(min, max));
    }

    bool near_zero() const
    {
        // Return true if the vector is close to zero in all dimensions.
        const auto s = 1e-8;
        return (fabs(x) < s) && (fabs(y) < s) && (fabs(z) < s);
    }

    vec3 safe_sqrt_vec()
    {
        return vec3(safe_sqrt(x), safe_sqrt(y), safe_sqrt(z));
    }

public:
    float x, y, z;
};

// Type aliases for vec3
using point3 = vec3; // 3D point
using color = vec3;  // RGB color

// vec3 Utility Functions

inline std::ostream &operator<<(std::ostream &out, const vec3 &v)
{
    return out << v.x << ' ' << v.y << ' ' << v.z;
}

inline vec3 operator+(const vec3 &u, const vec3 &v)
{
    return vec3(u.x + v.x, u.y + v.y, u.z + v.z);
}

inline vec3 operator-(const vec3 &u, const vec3 &v)
{
    return vec3(u.x - v.x, u.y - v.y, u.z - v.z);
}

inline vec3 operator*(const vec3 &u, const vec3 &v)
{
    return vec3(u.x * v.x, u.y * v.y, u.z * v.z);
}

inline vec3 operator*(double t, const vec3 &v)
{
    return vec3(t * v.x, t * v.y, t * v.z);
}

inline vec3 operator*(const vec3 &v, double t)
{
    return t * v;
}

inline vec3 operator/(vec3 v, double t)
{
    return (1 / t) * v;
}

inline vec3 operator/(vec3 v, vec3 w)
{
    return vec3(v[0] / w[0], v[1] / w[1], v[2] / w[2]);
}

inline double dot(const vec3 &u, const vec3 &v)
{
    return u.x * v.x + u.y * v.y + u.z * v.z;
}

inline vec3 cross(const vec3 &u, const vec3 &v)
{
    return vec3(u.y * v.z - u.z * v.y,
                u.z * v.x - u.x * v.z,
                u.x * v.y - u.y * v.x);
}

inline vec3 unit_vector(vec3 v)
{
    return v / v.length();
}

inline vec3 normalize(vec3 v)
{
    return v / v.length();
}

inline vec3 Lerp(vec3 a, vec3 b, float t)
{
    return a + (b - a) * t;
}

vec3 random_in_unit_sphere()
{
    while (true)
    {
        auto p = vec3::random(-1, 1);
        if (p.length_squared() >= 1)
            continue;
        return p;
    }
}

vec3 random_unit_vector()
{
    return unit_vector(random_in_unit_sphere());
}

vec3 random_in_hemisphere(const vec3 &normal)
{
    vec3 in_unit_sphere = random_in_unit_sphere();
    if (dot(in_unit_sphere, normal) > 0.0) // In the same hemisphere as the normal
        return in_unit_sphere;
    else
        return -in_unit_sphere;
}

vec3 reflect(const vec3 &v, const vec3 &n)
{
    return v - 2 * dot(v, n) * n;
}

vec3 refract(const vec3 &uv, const vec3 &n, double etai_over_etat)
{
    auto cos_theta = fmin(dot(-uv, n), 1.0);
    vec3 r_out_perp = etai_over_etat * (uv + cos_theta * n);
    vec3 r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.length_squared())) * n;
    return r_out_perp + r_out_parallel;
}

vec3 random_in_unit_disk()
{
    while (true)
    {
        auto p = vec3(random_double(-1, 1), random_double(-1, 1), 0);
        if (p.length_squared() >= 1)
            continue;
        return p;
    }
}

// remember to pass radians not angles
inline vec3 SphericalDirection(const float &sinTheta, const float &cosTheta, const float &phi)
{
    return vec3(sinTheta * std::cos(phi), sinTheta * std::sin(phi), cosTheta);
}

// remember to pass radians not angles
inline vec3 SphericalDirection(const float &sinTheta, const float &cosTheta, const float &phi, const vec3 &x, const vec3 &y, const vec3 &z)
{
    return sinTheta * std::cos(phi) * x + sinTheta * std::sin(phi) * y + cosTheta * z;
}

inline float SphericalTheta(const vec3 &v)
{
    return (std::acos(clamp(v.z, -1, 1)));
}

inline float SphericalPhi(const vec3 &v)
{
    float p = std::atan2(v.y, v.x);
    return (p < 0) ? (p + 2 * pi) : p;
}

inline vec3 random_cosine_direction() 
{
    auto r1 = random_double();
    auto r2 = random_double();
    auto z = sqrt(1-r2);

    auto phi = 2*pi*r1;
    auto x = cos(phi)*sqrt(r2);
    auto y = sin(phi)*sqrt(r2);

    return vec3(x, y, z);
}

inline vec3 random_to_sphere(double radius, double distance_squared)
{
    auto r1 = random_double();
    auto r2 = random_double();
    auto z = 1 + r2 * (sqrt(1 - radius * radius / distance_squared) - 1);

    auto phi = 2 * pi * r1;
    auto x = cos(phi) * sqrt(1 - z * z);
    auto y = sin(phi) * sqrt(1 - z * z);

    return vec3(x, y, z);
}

#endif