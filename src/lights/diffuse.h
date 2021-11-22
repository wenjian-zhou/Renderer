#ifndef LIGHT_DIFFUSE_H
#define LIGHT_DIFFUSE_H

#include "../core/light.h"

class DiffuseAreaLight : public AreaLight {
public:
    DiffuseAreaLight(const Spectrum &Le, int nSamples, const std::shared_ptr<Object> &object, bool twoSided = false);
    Spectrum L(const HitRecord &intr, const Vector3f &w) const {
        return (twoSided || Dot(intr.normal, w) > 0) ? Lemit : Spectrum(0.f);
    }
    Spectrum Power() const;
    Spectrum Sample_Li(const HitRecord &ref, const Point2f &u, Vector3f *wo, float *pdf, VisibilityTester *vis) const;
    float Pdf_Li(const HitRecord &, const Vector3f &) const;
    Spectrum Sample_Le(const Point2f &u1, const Point2f &u2, float time, Ray *ray, Vector3f *nLight, float *pdfPos, float *pdfDir) const;
    void Pdf_Le(const Ray &, const Vector3f &, float *pdfPos, float *pdfDir) const;
protected:
    const Spectrum Lemit;
    std::shared_ptr<Object> shape;
    const bool twoSided;
    const float area;
};

#endif