#ifndef VEC3_H
#define VEC3_H

#include "global.h"

#include <assert.h>
#include <cmath>
#include <iostream>

using std::sqrt;

template <typename T> class Vector2 {
public:
    Vector2() { x = y = 0; }
    Vector2(T xx, T yy) : x(xx), y(yy) {}
    bool HasNaNs() const {
        return std::isnan(x) || std::isnan(y);
    }

    Vector2(const Vector2<T> &v) {
        x = v.x; y = v.y;
    }
    Vector2<T> &operator=(const Vector2<T> &v) {
        x = v.x; y = v.y;
        return *this;
    }
    friend std::ostream& operator<<(std::ostream& os, const Vector2<T> &v) {
        os << "[" << v.x << ", " << v.y << "]";
        return os;
    }

    Vector2<T> operator+(const Vector2<T> &v) const {
        return Vector2(x + v.x, y + v.y);
    }

    Vector2<T>& operator+=(const Vector2<T> &v) {
        x += v.x; y += v.y;
        return *this;
    }

    Vector2<T> operator-(const Vector2<T> &v) const {
        return Vector2(x - v.x, y - v.y);
    }

    Vector2<T>& operator-=(const Vector2<T> &v) {
        x -= v.x; y -= v.y;
        return *this;
    }
    bool operator==(const Vector2<T> &v) const {
        return x == v.x && y == v.y;
    }
    bool operator!=(const Vector2<T> &v) const {
        return x != v.x || y != v.y;
    }
    Vector2<T> operator*(T f) const { return Vector2<T>(f*x, f*y); }

    Vector2<T> &operator*=(T f) {
        x *= f; y *= f;
        return *this;
    }

    Vector2<T> operator/(T f) const {
        float inv = 1.f / f;
        return Vector2<T>(x * inv, y * inv);
    }

    Vector2<T> &operator/=(T f) {
        float inv = 1.f / f;
        x *= inv; y *= inv;
        return *this;
    }

    Vector2<T> operator-() const { return Vector2<T>(-x, -y); }
    T operator[](int i) const {
        if (i == 0) return x;
        return y;
    }

    T &operator[](int i) {
        if (i == 0) return x;
        return y;
    }

    float LengthSquared() const { return x * x + y * y; }
    float Length() const { return std::sqrt(LengthSquared()); }
    
public:
    T x, y;
};

template <typename T> Vector2<T>
Lerp(float t, const Vector2<T> &p0, const Vector2<T> &p1);

template <typename T> class Vector3 {
public:
    T operator[](int i) const {
        if (i == 0) return x;
        if (i == 1) return y;
        return z;
    }
    T &operator[](int i) {
        if (i == 0) return x;
        if (i == 1) return y;
        return z;
    }

    Vector3() { x = y = z = 0; }
    Vector3(T xx) : x(xx), y(xx), z(xx) {}
    Vector3(T x, T y, T z) : x(x), y(y), z(z) {}
    Vector3(const Vector3<T> &v) {
        x = v.x; y = v.y; z = v.z;
    }

    bool HasNaNs() const {
        return std::isnan(x) || std::isnan(y) || std::isnan(z);
    }

    Vector3<T> &operator=(const Vector3<T> &v) {
        x = v.x; y = v.y; z = v.z;
        return *this;
    }

    friend std::ostream& operator<<(std::ostream& os, const Vector3<T> &v) {
        os << "[" << v.x << ", " << v.y << ", " << v.z << "]";
        return os;
    }

    Vector3<T> operator+(const Vector3<T> &v) const {
        return Vector3(x + v.x, y + v.y, z + v.z);
    }
    Vector3<T>& operator+=(const Vector3<T> &v) {
        x += v.x; y += v.y; z += v.z;
        return *this;
    }

    Vector3<T> operator-(const Vector3<T> &v) const {
        return Vector3(x - v.x, y - v.y, z - v.z);
    }
    Vector3<T>& operator-=(const Vector3<T> &v) {
        x -= v.x; y -= v.y; z -= v.z;
        return *this;
    }

    bool operator==(const Vector3<T> &v) const {
        return x == v.x && y == v.y && z == v.z;
    }
    bool operator!=(const Vector3<T> &v) const {
        return x != v.x || y != v.y || z != v.z;
    }
    Vector3<T> operator*(T s) const { return Vector3<T>(s * x, s * y, s * z); }
    Vector3<T> &operator*=(T s) {
        x *= s; y *= s; z *= s;
        return *this;
    }

    Vector3<T> operator/(T f) const {
        float inv = 1.f / f;
        return Vector3<T>(x * inv, y * inv, z * inv);
    }
    Vector3<T>& operator/=(T f) {
        float inv = 1.f / f;
        x *= inv; y *= inv; z *= inv;
        return *this;
    }

    Vector3<T> operator-() const { return Vector3<T>(-x, -y, -z); }
    float LengthSquared() const { return x * x + y * y + z * z; }
    float Length() const { return std::sqrt(LengthSquared()); }
public:
    T x, y, z;
};

template <typename T> inline Vector3<T>
operator*(T s, const Vector3<T> &v);
template <typename T> Vector3<T> Abs(const Vector3<T> &v);
template <typename T> inline T 
Dot(const Vector3<T> &v1, const Vector3<T> &v2);
template <typename T>
inline T AbsDot(const Vector3<T> &v1, const Vector3<T> &v2);
template <typename T> inline Vector3<T>
Cross(const Vector3<T> &v1, const Vector3<T> &v2);
template <typename T> inline Vector3<T>
Normalize(const Vector3<T> &v);
template <typename T> T
MinComponent(const Vector3<T> &v);
template <typename T> T
MaxComponent(const Vector3<T> &v);
template <typename T> Vector3<T> 
Min(const Vector3<T> &p1, const Vector3<T> &p2);
template <typename T> Vector3<T>
Max(const Vector3<T> &p1, const Vector3<T> &p2);
template <typename T> Vector3<T>
Permute(const Vector3<T> &v, int x, int y, int z);
template <typename T> inline void
CoordinateSystem(const Vector3<T> &v1, Vector3<T> *v2, Vector3<T> *v3);
template <typename T> inline float
Distance(const Vector3<T> &p1, const Vector3<T> &p2);
template <typename T> Vector3<T>
Lerp(float t, const Vector3<T> &p0, const Vector3<T> &p1);
template <typename T> Vector3<T> Floor(const Vector3<T> &p);
template <typename T> Vector3<T> Ceil(const Vector3<T> &p);
template <typename T> inline Vector3<T>
Faceforward(const Vector3<T> &n, const Vector3<T> &v);

typedef Vector2<float> Vector2f;
typedef Vector2<int> Vector2i;
typedef Vector3<float> Vector3f;
typedef Vector3<int> Vector3i;

inline Vector3f operator*(const Vector3f &u, const Vector3f &v)
{
    return Vector3f(u.x * v.x, u.y * v.y, u.z * v.z);
}

inline Vector3f operator/(Vector3f v, Vector3f w)
{
    return Vector3f(v[0] / w[0], v[1] / w[1], v[2] / w[2]);
}

template <typename T> class Bounds2 {
public:
    Bounds2() {
        T minNum = std::numeric_limits<T>::lowest();
        T maxNum = std::numeric_limits<T>::max();
        pMin = Vector2<T>(maxNum, maxNum);
        pMax = Vector2<T>(minNum, minNum);
    }
    Bounds2(const Vector2<T> &p) : pMin(p), pMax(p) {}
    Bounds2(const Vector2<T> &p1, const Vector2<T> &p2) {
        pMin = Vector2<T>(std::min(p1.x, p2.x), std::min(p1.y, p2.y));
        pMax = Vector2<T>(std::max(p1.x, p2.x), std::max(p1.y, p2.y));
    }
    template <typename U> explicit operator Bounds2<U>() const {
        return Bounds2<U>((Vector2<U>)pMin, (Vector2<U>)pMax);
    }

    Vector2<T> Diagonal() const {
        return pMax - pMin;
    }

    T Area() const {
        Vector2<T> d = pMax - pMin;
        return (d.x * d.y);
    }
    int MaximumExtent() const {
        Vector2<T> diag = Diagonal();
        if (diag.x > diag.y)
            return 0;
        else 
            return 1;
    }
    inline const Vector2<T> & operator[](int i) const {
        return (i == 0) ? pMin : pMax;
    }
    inline Vector2<T> &operator[](int i) {
        return (i == 0) ? pMin : pMax;
    }
    bool operator==(const Bounds2<T> &b) const {
        return b.pMin == pMin && b.pMax == pMax;
    }
    bool operator!=(const Bounds2<T> &b) const {
        return b.pMin != pMin || b.pMax != pMax;
    }

    Vector2<T> Lerp(const Vector2f &t) const {
        return Vector2<T>(::Lerp(t.x, pMin.x, pMax.x), ::Lerp(t.y, pMin.y, pMax.y));
    }
    Vector2<T> Offset(const Vector2<T> &p) const {
        Vector2<T> o = p - pMin;
        if (pMax.x > pMin.x) o.x /= pMax.x - pMin.x;
        if (pMax.y > pMin.y) o.y /= pMax.y - pMin.y;
        return o;
    }
public:
    Vector2<T> pMin, pMax;
};

template <typename T> class Bounds3 {
public:
    Bounds3() {
        T minNum = std::numeric_limits<T>::lowest();
        T maxNum = std::numeric_limits<T>::max();
        pMin = Vector3<T>(maxNum, maxNum, maxNum);
        pMax = Vector3<T>(minNum, minNum, minNum);
    }
    Bounds3(const Vector3<T> &p) : pMin(p), pMax(p) {}
    Bounds3(const Vector3<T> &p1, const Vector3<T> &p2)
        : pMin(std::min(p1.x, p2.x), std::min(p1.y, p2.y),
               std::min(p1.z, p2.z)),
          pMax(std::max(p1.x, p2.x), std::max(p1.y, p2.y),
               std::max(p1.z, p2.z)) {}
    const Vector3<T> &operator[](int i) const;
    Vector3<T> &operator[](int i);
    bool operator==(const Bounds3<T> &b) const {
        return b.pMin == pMin && b.pMax == pMax;
    }
    bool operator!=(const Bounds3<T> &b) const {
        return b.pMin != pMin || b.pMax != pMax;
    }
    Vector3<T> Corner(int corner) const {
        return Vector3<T>((*this)[(corner & 1)].x,
                        (*this)[(corner & 2) ? 1 : 0].y,
                        (*this)[(corner & 4) ? 1 : 0].z);
    }
    Vector3<T> Diagonal() const { return pMax - pMin; }
    T SurfaceArea() const {
        Vector3<T> d = Diagonal();
        return 2 * (d.x * d.y + d.x * d.z + d.y * d.z);
    }
    T Volume() const {
        Vector3<T> d = Diagonal();
        return d.x * d.y * d.z;
    }
    int MaximumExtent() const {
        Vector3<T> d = Diagonal();
        if (d.x > d.y && d.x > d.z)
            return 0;
        else if (d.y > d.z)
            return 1;
        else
            return 2;
    }
    Vector3<T> Lerp(const Vector3f &t) const {
        return Vector3<T>(::Lerp(t.x, pMin.x, pMax.x),
                        ::Lerp(t.y, pMin.y, pMax.y),
                        ::Lerp(t.z, pMin.z, pMax.z));
    }
    Vector3<T> Offset(const Vector3<T> &p) const {
        Vector3<T> o = p - pMin;
        if (pMax.x > pMin.x) o.x /= pMax.x - pMin.x;
        if (pMax.y > pMin.y) o.y /= pMax.y - pMin.y;
        if (pMax.z > pMin.z) o.z /= pMax.z - pMin.z;
        return o;
    }
    void BoundingSphere(Vector3<T> *center, float *radius) const {
        *center = (pMin + pMax) / 2;
        *radius = Inside(*center, *this) ? Distance(*center, pMax) : 0;
    }
    template <typename U> explicit operator Bounds3<U>() const {
        return Bounds3<U>((Vector3<U>)pMin, (Vector3<U>)pMax);
    }
    bool IntersectP(const Ray &ray, float *hitt0 = nullptr, float *hitt1 = nullptr) const;
    inline bool IntersectP(const Ray &ray, const Vector3f &invDir,
                            const int dirIsNeg[3]) const;
public:
    Vector3<T> pMin, pMax;
};

template <typename T> Bounds3 <T>
Union(const Bounds3<T> &b, const Vector3<T> &p);
template <typename T> Bounds3<T>
Union(const Bounds3<T> &b1, const Bounds3<T> &b2);
template <typename T> Bounds3<T>
Intersect(const Bounds3<T> &b1, const Bounds3<T> &b2);
template <typename T>
bool Overlaps(const Bounds3<T> &b1, const Bounds3<T> &b2);
template <typename T>
bool Inside(const Vector3<T> &p, const Bounds3<T> &b);
template <typename T>
bool InsideExclusive(const Vector3<T> &p, const Bounds3<T> &b);
template <typename T, typename U> inline Bounds3<T>
Expand(const Bounds3<T> &b, U delta);

typedef Bounds2<float> Bounds2f;
typedef Bounds2<int>   Bounds2i;
typedef Bounds3<float> Bounds3f;
typedef Bounds3<int>   Bounds3i;

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
using Point3i = Vector3i;
using Color = Vector3f;  // RGB color
using Point2f = Vector2f;

// Vector3f Utility Functions

Vector3f random_in_unit_sphere()
{
    while (true)
    {
        auto p = Vector3f(1.f, 1.f, 1.f);
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