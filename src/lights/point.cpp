#include "point.h"

Spectrum PointLight::Sample_Li(const HitRecord &ref, const Point2f &u, Vector3f *wi, float *pdf, VisibilityTester *vis) const {
    *wi = Normalize(pLight - ref.p);
    *pdf = 1.f;
    *vis = VisibilityTester(ref.p, pLight);
    return I / (Distance(pLight, ref.p) * Distance(pLight, ref.p));
}

float PointLight::Pdf_Li(const HitRecord &ref, const Vector3f &wi) const {
    return 0;
}