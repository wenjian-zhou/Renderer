#include "integrator.h"

Spectrum UniformSampleOneLight(const HitRecord &it, const Scene &scene, Sampler &sampler) {
    int nLights = int(scene.lights.size());
    if (nLights == 0) return Spectrum(0.f);
    int lightNum = std::min((int)(sampler.Next1D() * nLights), nLights - 1);
    const std::shared_ptr<Light> &light = scene.lights[lightNum];

    return EstimateDirect(it, *light, scene, sampler, false, false) / 1.f;
}

Spectrum EstimateDirect(const HitRecord &it, const Light &light, const Scene &scene, Sampler &sampler, bool handleMedia, bool specular) {
    BxDFType bsdfFlags = specular ? BSDF_ALL :
                         BxDFType(BSDF_ALL & ~BSDF_SPECULAR);
    Spectrum Ld(0.f);
    Vector3f wi;
    float lightPdf = 0, scatteringPdf = 0;
    VisibilityTester visibility;
    Spectrum Li = light.Sample_Li(it, sampler.Next2D(), &wi, &lightPdf, &visibility);
    
    if (lightPdf > 0 && !Li.IsBlack()) {
        Spectrum f;
        if (it.IsSurface()) {
            const HitRecord &isect = (const HitRecord &)it;   
            f = isect.bsdf->f(isect.wo, wi, bsdfFlags) * AbsDot(wi, isect.normal);
            scatteringPdf = isect.bsdf->Pdf(isect.wo, wi, bsdfFlags); 
        }
        // TODO else if : medium case

        if (!f.IsBlack()) {
            if (!visibility.Unoccluded(scene)) {
                Li = Spectrum(0.f);
            }
            // TODO else if(handleMedia)

            if (!Li.IsBlack()) {
                if (IsDelta(light.flags)) {
                    Ld += f * Li / lightPdf;
                }
                else {
                    float weight = PowerHeuristic(1, lightPdf, 1, scatteringPdf);
                    Ld += f * Li * weight / lightPdf;
                }
            }
        }

        if (!IsDelta(light.flags)) {
            Spectrum f;
            bool sampledSpecular = false;
            if (it.IsSurface()) {
                BxDFType sampledType;
                const HitRecord &isect = (const HitRecord &)it;
                f = isect.bsdf->Sample_f(isect.wo, &wi, sampler.Next2D(), &scatteringPdf, bsdfFlags, &sampledType);
                f *= AbsDot(wi, isect.normal);
                sampledSpecular = (sampledType & BSDF_SPECULAR) != 0;
            }
            else {
                // medium
            }

            if (!f.IsBlack() && scatteringPdf > 0) {
                float weight = 1;
                if (!sampledSpecular) {
                    lightPdf = light.Pdf_Li(it, wi);
                    if (lightPdf == 0) return Ld;
                    weight = PowerHeuristic(1, scatteringPdf, 1, lightPdf);
                }

                HitRecord lightIsect;
                Ray ray = Ray(it.p, wi);
                Spectrum Tr(1.f);
                bool foundSurfaceInteraction = handleMedia ? scene.IntersectTr(ray, sampler, lightIsect, &Tr)
                                                           : scene.Intersect(ray, lightIsect);

                Spectrum Li(0.f);
                if (foundSurfaceInteraction) {
                    
                } else {
                    Li = light.Le(ray);
                }
                if (!Li.IsBlack()) Ld += f * Li * Tr * weight / scatteringPdf;
            }
        }
    }
    return Ld;
}