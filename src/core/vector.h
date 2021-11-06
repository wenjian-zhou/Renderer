#ifndef VEC3_H
#define VEC3_H

#include "global.h"

#include <assert.h>
#include <cmath>
#include <iostream>

using std::sqrt;

class Vector2f {
public:
    Vector2f() { x = y = 0; }
    Vector2f(float xx) : x(xx), y(xx) {}
    Vector2f(float xx, float yy) : x(xx), y(yy) {}
public:
    float x, y;
};

class Vector3f
{
public:
    Vector3f() { x = y = z = 0; }
    Vector3f(float xx) : x(xx), y(xx), z(xx) {}
    Vector3f(float xx, float yy, float zz) : x(xx), y(yy), z(zz) {}
    Vector3f(const Vector2f &v, float zz) : x(v.x), y(v.y), z(zz) {}

    Vector3f operator-() const { return Vector3f(-x, -y, -z); }
    float operator[](int i) const { return (i == 0) ? x : (i == 1) ? y : z; }
    float &operator[](int i) { return (i == 0) ? x : (i == 1) ? y : z; }

    Vector3f &operator+=(const Vector3f &v)
    {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }

    Vector3f &operator*=(const double t)
    {
        x *= t;
        y *= t;
        z *= t;
        return *this;
    }

    Vector3f &operator/=(const double t)
    {
        return *this *= 1 / t;
    }

    double Length() const
    {
        return sqrt(LengthSquared());
    }

    double LengthSquared() const
    {
        return x * x + y * y + z * z;
    }

    inline static Vector3f Random()
    {
        return Vector3f(random_double(), random_double(), random_double());
    }

    inline static Vector3f Random(double min, double max)
    {
        return Vector3f(random_double(min, max), random_double(min, max), random_double(min, max));
    }

    bool near_zero() const
    {
        // Return true if the vector is close to zero in all dimensions.
        const auto s = 1e-8;
        return (fabs(x) < s) && (fabs(y) < s) && (fabs(z) < s);
    }

    Vector3f safe_sqrt_vec()
    {
        return Vector3f(safe_sqrt(x), safe_sqrt(y), safe_sqrt(z));
    }

public:
    float x, y, z;
};

class Vector4f {
public:
    Vector4f() { x = y = z = w = 0; }
    Vector4f(float xx) : x(xx), y(xx), z(xx), w(xx) {}
    Vector4f(float xx, float yy, float zz, float ww) : x(xx), y(yy), z(zz), w(ww) {}
    Vector4f(const Vector3f &v, float w) : x(v.x), y(v.y), z(v.z), w(w) {}

    Vector4f operator*(const float m) const { return Vector4f(m * x, m * y, m * z, m * w); }
    Vector4f &operator/=(const float m) {
        x /= m;
        y /= m;
        z /= m;
        w /= m;
        return *this;
    }

    float operator[](int i) const { return (i == 0) ? x : (i == 1) ? y : (i == 2) ? z : w; }
    float &operator[](int i) { return (i == 0) ? x : (i == 1) ? y : (i == 2) ? z : w; }
public:
    float x, y, z, w;
};

// Type aliases for Vector3f
using Point3f = Vector3f; // 3D point
using Color = Vector3f;  // RGB color

// Vector3f Utility Functions

inline std::ostream &operator<<(std::ostream &out, const Vector3f &v)
{
    return out << v.x << ' ' << v.y << ' ' << v.z;
}

inline Vector3f operator+(const Vector3f &u, const Vector3f &v)
{
    return Vector3f(u.x + v.x, u.y + v.y, u.z + v.z);
}

inline Vector3f operator-(const Vector3f &u, const Vector3f &v)
{
    return Vector3f(u.x - v.x, u.y - v.y, u.z - v.z);
}

inline Vector3f operator*(const Vector3f &u, const Vector3f &v)
{
    return Vector3f(u.x * v.x, u.y * v.y, u.z * v.z);
}

inline Vector3f operator*(double t, const Vector3f &v)
{
    return Vector3f(t * v.x, t * v.y, t * v.z);
}

inline Vector3f operator*(const Vector3f &v, double t)
{
    return t * v;
}

inline Vector3f operator/(Vector3f v, double t)
{
    return (1 / t) * v;
}

inline Vector3f operator/(Vector3f v, Vector3f w)
{
    return Vector3f(v[0] / w[0], v[1] / w[1], v[2] / w[2]);
}

inline float Dot(const Vector3f &u, const Vector3f &v)
{
    return u.x * v.x + u.y * v.y + u.z * v.z;
}

inline float AbsDot(const Vector3f &u, const Vector3f &v) {
    return std::abs(Dot(u, v));
}

inline Vector3f Cross(const Vector3f &u, const Vector3f &v)
{
    return Vector3f(u.y * v.z - u.z * v.y,
                u.z * v.x - u.x * v.z,
                u.x * v.y - u.y * v.x);
}

inline Vector3f Normalize(Vector3f v)
{
    return v / v.Length();
}

inline Vector3f Lerp(Vector3f a, Vector3f b, float t)
{
    return a + (b - a) * t;
}

Vector3f random_in_unit_sphere()
{
    while (true)
    {
        auto p = Vector3f::Random(-1, 1);
        if (p.LengthSquared() >= 1)
            continue;
        return p;
    }
}

Vector3f random_unit_vector()
{
    return Normalize(random_in_unit_sphere());
}

Vector3f random_in_hemisphere(const Vector3f &normal)
{
    Vector3f in_unit_sphere = random_in_unit_sphere();
    if (Dot(in_unit_sphere, normal) > 0.0) // In the same hemisphere as the normal
        return in_unit_sphere;
    else
        return -in_unit_sphere;
}

Vector3f Reflect(const Vector3f &v, const Vector3f &n)
{
    return v - 2 * Dot(v, n) * n;
}

Vector3f Refract(const Vector3f &uv, const Vector3f &n, double etai_over_etat)
{
    auto cos_theta = fmin(Dot(-uv, n), 1.0);
    Vector3f r_out_perp = etai_over_etat * (uv + cos_theta * n);
    Vector3f r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.LengthSquared())) * n;
    return r_out_perp + r_out_parallel;
}

Vector3f random_in_unit_disk()
{
    while (true)
    {
        auto p = Vector3f(random_double(-1, 1), random_double(-1, 1), 0);
        if (p.LengthSquared() >= 1)
            continue;
        return p;
    }
}

// remember to pass radians not angles
inline Vector3f SphericalDirection(const float &sinTheta, const float &cosTheta, const float &phi)
{
    return Vector3f(sinTheta * std::cos(phi), sinTheta * std::sin(phi), cosTheta);
}

// remember to pass radians not angles
inline Vector3f SphericalDirection(const float &sinTheta, const float &cosTheta, const float &phi, const Vector3f &x, const Vector3f &y, const Vector3f &z)
{
    return sinTheta * std::cos(phi) * x + sinTheta * std::sin(phi) * y + cosTheta * z;
}

inline float SphericalTheta(const Vector3f &v)
{
    return (std::acos(clamp(v.z, -1, 1)));
}

inline float SphericalPhi(const Vector3f &v)
{
    float p = std::atan2(v.y, v.x);
    return (p < 0) ? (p + 2 * PI) : p;
}

inline Vector3f random_cosine_direction() 
{
    auto r1 = random_double();
    auto r2 = random_double();
    auto z = sqrt(1-r2);

    auto phi = 2*PI*r1;
    auto x = cos(phi)*sqrt(r2);
    auto y = sin(phi)*sqrt(r2);

    return Vector3f(x, y, z);
}

inline Vector3f random_to_sphere(double radius, double distance_squared)
{
    auto r1 = random_double();
    auto r2 = random_double();
    auto z = 1 + r2 * (sqrt(1 - radius * radius / distance_squared) - 1);

    auto phi = 2 * PI * r1;
    auto x = cos(phi) * sqrt(1 - z * z);
    auto y = sin(phi) * sqrt(1 - z * z);

    return Vector3f(x, y, z);
}

#endif