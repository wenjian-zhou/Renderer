#ifndef VECTOR_H
#define VECTOR_H

#include <cmath>
#include <iostream>

using std::sqrt;

class Vector3f
{
public:
    float e[3];

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

#endif