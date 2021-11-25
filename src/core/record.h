#ifndef RECORD_H
#define RECORD_H

#include "ray.h"
#include "spectrum.h"
#include "medium.h"

struct MediumRecord {
    MediumRecord() { inside = outside = nullptr; }
    std::shared_ptr<Medium> inside, outside;
    MediumRecord(std::shared_ptr<Medium> medium) : inside(medium), outside(medium) {}
    MediumRecord(std::shared_ptr<Medium> inside, std::shared_ptr<Medium> outside) : inside(inside), outside(outside) {}
    // bool IsMediumTransition const { return inside != outside; }
    bool IsValid() const { return phase != nullptr; }
    std::shared_ptr<PhaseFunction> phase;
};

struct HitRecord
{
    HitRecord() {}
    Point3f p;
    Vector3f normal;
    Vector3f wo, wi;
    shared_ptr<Material> mat_ptr = nullptr;
    BSDF *bsdf = nullptr;
    double t;
    double u, v;
    Spectrum Le = 0.f;
    bool front_face;
    MediumRecord mediumRecord;

    std::shared_ptr<Material> GetMaterial() { return mat_ptr; }
    const std::shared_ptr<Medium> GetMedium(const Vector3f &d) {
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