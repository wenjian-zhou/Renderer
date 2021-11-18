#ifndef RECORD_H
#define RECORD_H

#include "global.h"
#include "ray.h"

struct MediumRecord {
    MediumRecord() {}
    const Medium *inside, *outside;
    MediumRecord(const Medium *medium) : inside(medium), outside(medium) {}
    MediumRecord(const Medium *inside, const Medium *outside) : inside(inside), outside(outside) {}
    // bool IsMediumTransition const { return inside != outside; }
};

struct HitRecord
{
    HitRecord() {}
    Point3f p;
    Vector3f normal;
    Vector3f wo, wi;
    shared_ptr<Material> mat_ptr;
    BSDF *bsdf;
    double t;
    double u, v;
    Spectrum Le;
    bool front_face;
    MediumRecord mediumRecord;

    std::shared_ptr<Material> GetMaterial() { return mat_ptr; }
    const Medium *GetMedium(const Vector3f &d) {
        return (Dot(normal, d) > 0) ? mediumRecord.outside : mediumRecord.inside;
    }

    inline void set_face_normal(const Ray &r, const Vector3f &outward_normal)
    {
        front_face = Dot(r.d, outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }

    bool IsSurface() const {
        return normal != Vector3f(0.f);
    }
};

#endif