#ifndef VECTOR_H
#define VECTOR_H

#include <cmath>
#include <iostream>
#include <algorithm>

class Vector2f
{
public:
    Vector2f() : x(0), y(0) {}
    Vector2f(float xx) : x(xx), y(xx) {}
    Vector2f(float xx, float yy) : x(xx), y(yy) {}

    // vector-scaling operations
    Vector2f operator * (const float &s) const { return Vector2f(x * s, y * s); }
    Vector2f operator / (const float &s) const { return Vector2f(x / s, y / s); }

    // vector-vector operations
    Vector2f operator - () const { return Vector2f(-x, -y); }
    Vector2f operator + (const Vector2f &v) const { return Vector2f(x + v.x, y + v.y); }
    Vector2f operator - (const Vector2f &v) const { return Vector2f(x - v.x, y - v.y); }
    Vector2f operator * (const Vector2f &v) const { return Vector2f(x * v.x, y * v.y); }

    float operator [] (int i) const
    {
        if (i == 0) return x;
        return y;
    }

    float &operator [] (int i)
    {
        if (i == 0) return x;
        return y;
    }

public:
    float x, y;
};

class Vector3f
{
public:
    Vector3f() : x(0), y(0), z(0) {}
    Vector3f(float xx) : x(xx), y(xx), z(xx) {}
    Vector3f(float xx, float yy, float zz) : x(xx), y(yy), z(zz) {}

    // vector-scaling operations
    Vector3f operator * (const float &s) const { return Vector3f(x * s, y * s, z * s); }
    Vector3f operator / (const float &s) const { return Vector3f(x / s, y / s, z / s); }

    // vector-vector operations
    Vector3f operator + (const Vector3f &v) const { return Vector3f(x + v.x, y + v.y, z + v.z); }
    Vector3f operator - (const Vector3f &v) const { return Vector3f(x - v.x, y - v.y, z - v.z); }
    Vector3f operator - ()                  const { return Vector3f(-x, -y, -z); }
    Vector3f operator * (const Vector3f &v) const { return Vector3f(x * v.x, y * v.y, z * v.z); }
    Vector3f &operator += (const Vector3f &v)     { x += v.x, y += v.y, z += v.z; return *this; }
    Vector3f operator == (const Vector3f &v) const { return v.x == x && v.y == y && v.z == z; }
    Vector3f operator != (const Vector3f &v) const { return v.x != x || v.y != y || v.z != z; }

    float operator [] (int i) const
    {
        if (i == 0) return x;
        if (i == 1) return y;
        return z;
    }

    float &operator [] (int i)
    {
        if (i == 0) return x;
        if (i == 1) return y;
        return z;
    }

    float LengthSquared() { return x * x + y * y + z * z; }
    float Length() { return std::sqrt(x * x + y * y + z * z); }
    Vector3f Normalize()
    {
        float n = std::sqrt(x * x + y * y + z * z);
        return Vector3f(x / n, y / n, z / n);
    }

    float MinComponent()
    {
        return std::min(x, std::min(y, z));
    }

    float MaxComponent()
    {
        return std::max(x, std::max(y, z));
    }

    static Vector3f Min(const Vector3f &p1, const Vector3f &p2)
    {
        return Vector3f(std::min(p1.x, p2.x),
                        std::min(p1.y, p2.y),
                        std::min(p1.z, p2.z));
    }

    static Vector3f Max(const Vector3f &p1, const Vector3f &p2)
    {
        return Vector3f(std::max(p1.x, p2.x),
                        std::max(p1.y, p2.y),
                        std::max(p1.z, p2.z));
    }

public:
    float x, y, z;
};

inline float Dot(const Vector3f &v, const Vector3f &w)
{
    return v.x * w.x + v.y * w.y + v.z * w.z;
}

inline float AbsDot(const Vector3f &v, const Vector3f &w)
{
    return std::abs(Dot(v, w));
}

inline Vector3f Cross(const Vector3f &v, const Vector3f &w)
{
    float i = v.y * w.z - v.z * w.y;
    float j = v.z * w.x - v.x * w.z;
    float k = v.x * w.y - v.y * w.x;
    return Vector3f(i, j, k);
}

inline void CoordinateSystem(const Vector3f &v1, Vector3f *v2, Vector3f *v3)
{
    if (std::abs(v1.x) > std::abs(v1.y))
    {
        *v2 = Vector3f(-v1.z, 0, v1.x) / std::sqrt(v1.x * v1.x + v1.z * v1.z);
    }
    else
    {
        *v2 = Vector3f(0, v1.z, -v1.y) / std::sqrt(v1.y * v1.y + v1.z * v1.z);
    }
    *v3 = Cross(v1, *v2);
}

inline Vector3f Lerp(const float &t, const Vector3f &p0, const Vector3f &p1)
{
    return p0 * (1 - t) + p1 * t;
}

inline Vector3f Floor(const Vector3f &v)
{
    return Vector3f(std::floor(v.x), std::floor(v.y), std::floor(v.z));
}

inline Vector3f Ceil(const Vector3f &v)
{
    return Vector3f(std::ceil(v.x), std::ceil(v.y), std::ceil(v.z));
}

inline Vector3f Abs(const Vector3f &v)
{
    return Vector3f(std::abs(v.x), std::abs(v.y), std::abs(v.z));
}

inline Vector3f Permute(const Vector3f &v, int &x, int &y, int &z)
{
    return Vector3f(v[x], v[y], v[z]);
}

inline Vector3f Faceforward(const Vector3f &normal, const Vector3f &v)
{
    return (Dot(normal, v) < 0.f) ? -normal : normal;
}

#endif