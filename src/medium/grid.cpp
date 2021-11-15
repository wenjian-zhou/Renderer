#include "grid.h"

float GridDensityMedium::Density(const Point3f &p) const {
    Point3f pSamples(p.x * nx - .5f, p.y * ny - .5f, p.z * nz - .5f);
    Point3i pi = (Point3i)(Floor(pSamples).x, Floor(pSamples).y, Floor(pSamples).z);
    Vector3f d = pSamples - (Point3f)(pi.x, pi.y, pi.z);

    float d00 = Lerp(d.x, D(pi),                 D(pi+Vector3i(1,0,0)));
    float d10 = Lerp(d.x, D(pi+Vector3i(0,1,0)), D(pi+Vector3i(1,1,0)));
    float d01 = Lerp(d.x, D(pi+Vector3i(0,0,1)), D(pi+Vector3i(1,0,1)));
    float d11 = Lerp(d.x, D(pi+Vector3i(0,1,1)), D(pi+Vector3i(1,1,1)));
    float d0 = Lerp(d.y, d00, d10);
    float d1 = Lerp(d.y, d01, d11);
    return Lerp(d.z, d0, d1);
}