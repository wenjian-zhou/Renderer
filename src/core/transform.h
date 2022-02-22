#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "vector.h"

struct Matrix4x4 {
    Matrix4x4() {
        m[0][0] = m[1][1] = m[2][2] = m[3][3] = 1.f;
        m[0][1] = m[0][2] = m[0][3] = m[1][0] =
                m[1][2] = m[1][3] = m[2][0] = m[2][1] = m[2][3] =
                m[3][0] = m[3][1] = m[3][2] = 0.f;
    }
    Matrix4x4(float v) {
        for (int i = 0; i < 4; ++i)
            for(int j = 0; j < 4; ++j)
                m[i][j] = v;
    }
    Matrix4x4(float mat[4][4]);
    Matrix4x4(float t00, float t01, float t02, float t03,
                 float t10, float t11, float t12, float t13,
                 float t20, float t21, float t22, float t23,
                 float t30, float t31, float t32, float t33);
    bool operator==(const Matrix4x4 &m2) const {
        for (int i = 0; i < 4; ++i)
            for(int j = 0; j < 4; ++j)
                if (m[i][j] != m2.m[i][j]) return false;
        
        return true;
    }
    bool operator!=(const Matrix4x4 &m2) const {
        for (int i = 0; i < 4; ++i)
            for(int j = 0; j < 4; ++j)
                if (m[i][j] != m2.m[i][j]) return true;
        
        return false;
    }
    friend Matrix4x4 Transpose(const Matrix4x4 &);
    void Print(FILE *f) const {
        fprintf(f, "[ ");
        for (int i = 0; i < 4; ++i) {
            fprintf(f, "  [ ");
            for (int j = 0; j < 4; ++j)  {
                fprintf(f, "%f", m[i][j]);
                if (j != 3) fprintf(f, ", ");
            }
            fprintf(f, " ]\n");
        }
        fprintf(f, " ] ");
    }
    static Matrix4x4 Mul(const Matrix4x4 &m1, const Matrix4x4 &m2) {
        Matrix4x4 r;
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                r.m[i][j] = m1.m[i][0] * m2.m[0][j] + 
                            m1.m[i][1] * m2.m[1][j] + 
                            m1.m[i][2] * m2.m[2][j] + 
                            m1.m[i][3] * m2.m[3][j];
        return r;
    }
    friend Matrix4x4 Inverse(const Matrix4x4 &);

    float m[4][4];
};

inline Vector4f operator*(const Matrix4x4 &matrix, const Vector4f &v) {
    float tmp[4];
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            tmp[i] += v[j] * matrix.m[i][j];
        }
    }
    return Vector4f(tmp[0], tmp[1], tmp[2], tmp[3]);
}

class Transform {
public:
    Transform() : m(1.f), invM(1.f) {}
    Transform(const Matrix4x4 &_m) : m(_m), invM(Inverse(_m)) {}
    Transform(const Matrix4x4 &_m, const Matrix4x4 _invM) : m(_m), invM(_invM) {}

    Transform operator*(const Transform &t) const { return Transform(Matrix4x4::Mul(m, t.m), Matrix4x4::Mul(invM, t.invM)); }

    Vector3f TransformPoint(const Vector3f &p) const {
        Vector4f ret = m * Vector4f(p, 1.f);
        ret /= ret.w;
        return Vector3f(ret.x, ret.y, ret.z);
    }
public:
    Matrix4x4 m, invM;
};

Transform Inverse(const Transform &t);
Transform LookAt(const Vector3f &pos, const Vector3f &lookat, const Vector3f &up);
Transform Perspective(const float &fov, const float &width, const float &height);
Transform Scale(const float &x, const float &y, const float &z);
Transform Translate(const float &x, const float &y, const float &z);
Transform Transpose(const Transform &t);

#endif