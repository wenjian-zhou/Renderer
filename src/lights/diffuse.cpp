#include "diffuse.h"

DiffuseAreaLight::DiffuseAreaLight(const Spectrum &Le, int nSamples, const std::shared_ptr<Object> &object, bool twoSided)
    : AreaLight(nSamples), Lemit(Le), shape(object), twoSided(twoSided), area(object->area) {}

Spectrum DiffuseAreaLight::Power() const {
    return (twoSided ? 2 : 1) * Lemit * area * PI;
}

Spectrum DiffuseAreaLight::Sample_Li(const HitRecord &ref, const Point2f &u, Vector3f *wi, float *pdf, VisibilityTester *vis) const {
    Vector3f dir = shape->random(ref.p);
    *wi = Normalize(dir);
    *pdf = 1.f / shape->pdf_value(ref.p, *wi);
    HitRecord it;
    bool hit = shape->Intersect(Ray(ref.p, *wi), it);
    *vis = VisibilityTester(ref.p, it.p);
    if (*pdf == 0 || !hit) return Spectrum(0.f);
    //std::cout << shape->pdf_value(ref.p, *wi) << std::endl;
    return L(it, -*wi);
}

float DiffuseAreaLight::Pdf_Li(const HitRecord &ref, const Vector3f &wi) const {
    return 1.f / shape->pdf_value(ref.p, wi);
}