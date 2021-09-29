#ifndef REDNERER_VECTOR_H
#define RENDERER_VECTOR_H

#include "global.hpp"

class Vector3f
{
public:
    Vector3f() : x(0.f), y(0.f), z(0.f) {}
    Vector3f(float _xx) : x(_xx), y(_xx), z(_xx) {}
    Vector3f(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}

    Vector3f operator + (const Vector3f &v) const { return Vector3f(x + v.x, y + v.y, z + v.z); }
    Vector3f operator - () const { return Vector3f(-x, -y, -z); }
    Vector3f operator - (const Vector3f &v) const { return Vector3f(x - v.x, y - v.y, z - v.z); }
    Vector3f operator * (const Vector3f &v) const { return Vector3f(x * v.x, y * v.y, z * v.z); }
    Vector3f operator * (const float &s) const { return Vector3f(x * s, y * s, z * s); }
    Vector3f operator / (const Vector3f &v) const { return Vector3f(x / v.x, y / v.y, z / v.z); }
    Vector3f operator / (const float &s) const { return Vector3f(x / s, y / s, z / s); }

    float operator [] (int i) const { return i == 0 ? x : i == 1 ? y : z; }
    float &operator [] (int i) { return i == 0 ? x : i == 1 ? y : z; }

    Vector3f &operator += (const Vector3f &v) { x + v.x; y + v.y; z + v.z; return *this; }

    float LengthSquare() { return x * x + y * y + z * z; }
    float Length() { return SafeSqrt(LengthSquare()); }
    Vector3f Normalize() { return Vector3f(x, y, z) / Length(); }
public:
    float x, y, z;
};

inline float DotProduct(const Vector3f &v, const Vector3f &w)
{
    return v.x * w.x + v.y * w.y + v.z * w.z;
}

inline Vector3f CrossProduct(const Vector3f &v, const Vector3f &w)
{
    return Vector3f(
        v.y * w.z - v.z * w.y,
        v.z * w.x - v.x * w.z,
        v.x * w.y - v.y * w.z
    );
}

#endif