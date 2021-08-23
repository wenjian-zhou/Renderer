#ifndef VECTOR_H
#define VECTOR_H

#include <cmath>
#include <iostream>

using std::sqrt;

class Vector3f
{
public:
    Vector3f() : e{0, 0, 0} {}
    Vector3f(float v1, float v2, float v3) : e{v1, v2, v3} {}

    float x() const { return e[0]; }
    float y() const { return e[1]; }
    float z() const { return e[2]; }

    Vector3f operator-() const { return {-e[0], -e[1], -e[2]}; }
    float operator[](int i) const { return e[i]; }
    float &operator[](int i) { return e[i]; }

    Vector3f &operator+=(const Vector3f &v)
    {
        e[0] += v.e[0];
        e[1] += v.e[1];
        e[2] += v.e[2];

        return *this;
    }

    Vector3f &operator*=(const float t)
    {
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;

        return *this;
    }

    Vector3f &operator/=(const float t)
    {
        return *this *= 1.0 / t;
    }

    float length() const
    {
        return sqrt(length_squared());
    }

    float length_squared() const
    {
        return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
    }

    inline static Vector3f random()
    {
        return Vector3f(randomFloat(), randomFloat(), randomFloat());
    }

    inline static Vector3f random(float min, float max)
    {
        return Vector3f(randomFloat(min, max), randomFloat(min, max), randomFloat(min, max));
    }

    bool nearZero() const
    {
        const auto s = 1e-8;
        return (fabs(e[0]) < s) && (fabs(e[1]) < s) && (fabs(e[2]) < s);
    }

public:
    float e[3];
};

using Point3 = Vector3f;
using Color = Vector3f;

inline std::ostream& operator<<(std::ostream &out, const Vector3f &v)
{
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline Vector3f operator+(const Vector3f &u, const Vector3f &v)
{
    return Vector3f(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

inline Vector3f operator-(const Vector3f &u, const Vector3f &v)
{
    return Vector3f(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

inline Vector3f operator*(const Vector3f &u, const Vector3f &v)
{
    return Vector3f(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline Vector3f operator*(float t, const Vector3f &v)
{
    return Vector3f(t * v.e[0], t * v.e[1], t * v.e[2]);
}

inline Vector3f operator*(const Vector3f &v, float t)
{
    return t * v;
}

inline Vector3f operator/(const Vector3f &v, float t)
{
    return (1.0 / t) * v;
}

inline float dot(const Vector3f &u, const Vector3f &v)
{
    return u.e[0] * v.e[0]
         + u.e[1] * v.e[1]
         + u.e[2] * v.e[2];
}

inline Vector3f cross(const Vector3f &u, const Vector3f &v)
{
    return Vector3f(u.e[1] * v.e[2] - u.e[2] * v.e[1],
                    u.e[2] * v.e[0] - u.e[0] * v.e[2],
                    u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

inline Vector3f normalized(Vector3f v)
{
    return v / v.length();
}

Vector3f randomInUnitSphere()
{
    while (true)
    {
        auto p = Vector3f::random(-1, 1);
        if (p.length_squared() >= 1) continue;
        return p;
    }
}

Vector3f randomUnitVector()
{
    return normalized(randomInUnitSphere());
}

Vector3f randomInHemisphere(const Vector3f &normal)
{
    Vector3f in_unit_sphere = randomInUnitSphere();
    if (dot(in_unit_sphere, normal) > 0.0)
    {
        return in_unit_sphere;
    }
    else
    {
        return -in_unit_sphere;
    }
}

Vector3f reflect(const Vector3f &v, const Vector3f &n)
{
    return v - 2 * dot(v, n) * n;
}

Vector3f refract(const Vector3f &uv, const Vector3f &n, float etai_over_etat)
{
    auto cos_theta = fmin(dot(-uv, n), 1.0);
    Vector3f r_out_perp = etai_over_etat * (uv + cos_theta * n);
    Vector3f r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.length_squared())) * n;
    return r_out_perp + r_out_parallel;
}

Vector3f randomInUnitDisk()
{
    while (true)
    {
        auto p = Vector3f(randomFloat(-1, 1), randomFloat(-1, 1), 0);
        if (p.length_squared() >= 1) continue;
        return p;
    }
}

#endif