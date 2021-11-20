#include "transform.h"

Matrix4x4::Matrix4x4(float mat[4][4]) { memcpy(m, mat, 16 * sizeof(float)); }

Matrix4x4::Matrix4x4(float t00, float t01, float t02, float t03, float t10,
                     float t11, float t12, float t13, float t20, float t21,
                     float t22, float t23, float t30, float t31, float t32,
                     float t33) {
    m[0][0] = t00;
    m[0][1] = t01;
    m[0][2] = t02;
    m[0][3] = t03;
    m[1][0] = t10;
    m[1][1] = t11;
    m[1][2] = t12;
    m[1][3] = t13;
    m[2][0] = t20;
    m[2][1] = t21;
    m[2][2] = t22;
    m[2][3] = t23;
    m[3][0] = t30;
    m[3][1] = t31;
    m[3][2] = t32;
    m[3][3] = t33;
}

Matrix4x4 Transpose(const Matrix4x4 &m) {
    return Matrix4x4(m.m[0][0], m.m[1][0], m.m[2][0], m.m[3][0], m.m[0][1],
                     m.m[1][1], m.m[2][1], m.m[3][1], m.m[0][2], m.m[1][2],
                     m.m[2][2], m.m[3][2], m.m[0][3], m.m[1][3], m.m[2][3],
                     m.m[3][3]);
}

Matrix4x4 Inverse(const Matrix4x4 &m) {
    int indxc[4], indxr[4];
    int ipiv[4] = {0, 0, 0, 0};
    float minv[4][4];
    memcpy(minv, m.m, 4 * 4 * sizeof(float));
    for (int i = 0; i < 4; i++) {
        int irow = 0, icol = 0;
        float big = 0.f;
        // Choose pivot
        for (int j = 0; j < 4; j++) {
            if (ipiv[j] != 1) {
                for (int k = 0; k < 4; k++) {
                    if (ipiv[k] == 0) {
                        if (std::abs(minv[j][k]) >= big) {
                            big = float(std::abs(minv[j][k]));
                            irow = j;
                            icol = k;
                        }
                    } else if (ipiv[k] > 1)
                        std::cerr << "Singular matrix in MatrixInvert";
                }
            }
        }
        ++ipiv[icol];
        // Swap rows _irow_ and _icol_ for pivot
        if (irow != icol) {
            for (int k = 0; k < 4; ++k) std::swap(minv[irow][k], minv[icol][k]);
        }
        indxr[i] = irow;
        indxc[i] = icol;
        if (minv[icol][icol] == 0.f) std::cerr << "Singular matrix in MatrixInvert";

        // Set $m[icol][icol]$ to one by scaling row _icol_ appropriately
        float pivinv = 1. / minv[icol][icol];
        minv[icol][icol] = 1.;
        for (int j = 0; j < 4; j++) minv[icol][j] *= pivinv;

        // Subtract this row from others to zero out their columns
        for (int j = 0; j < 4; j++) {
            if (j != icol) {
                float save = minv[j][icol];
                minv[j][icol] = 0;
                for (int k = 0; k < 4; k++) minv[j][k] -= minv[icol][k] * save;
            }
        }
    }
    // Swap columns to reflect permutation
    for (int j = 3; j >= 0; j--) {
        if (indxr[j] != indxc[j]) {
            for (int k = 0; k < 4; k++)
                std::swap(minv[k][indxr[j]], minv[k][indxc[j]]);
        }
    }
    return Matrix4x4(minv);
}

Transform Inverse(const Transform &t) {
    return Transform(Inverse(t.m), Inverse(t.invM));
}

Transform Transpose(const Transform &t) {
    return Transform(Transpose(t.m), Transpose(t.invM));
}

Transform Scale(const float &x, const float &y, const float &z) {
    Matrix4x4 m(x, 0, 0, 0,
                0, y, 0, 0,
                0, 0, z, 0,
                0, 0, 0, 1);
    Matrix4x4 minv(1/x,   0,   0, 0,
                   0,   1/y,   0, 0,
                   0,     0, 1/z, 0,
                   0,     0,   0, 1);
    return Transform(m, minv);
}

Transform Perspective(const float &fov, const float &width, const float &height) {
    float rad = fov;
    float h = std::cos(0.5 * rad) / std::sin(0.5 * rad);
    float w = h * height / width;

    Matrix4x4 result(0.f);
    result.m[0][0] = w;
    result.m[1][1] = h;
    result.m[2][2] = - (10000.f + 0.001f) / (10000.f - 0.001f);
    result.m[2][3] = - 1.f;
    result.m[3][2] = - (2.f * 10000.f * 0.001f) / (10000.f - 0.001f);
    return Transform(result);
}