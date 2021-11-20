#include "integrator.h"

Spectrum UniformSampleOneLight(const HitRecord &it, const Scene &scene, Sampler &sampler) {
    int nLights = int(scene.lights.size());
    if (nLights == 0) return Spectrum(0.f);
    int lightNum = std::min((int)(sampler.Next1D() * nLights), nLights - 1);
    const std::shared_ptr<Light> &light = scene.lights[lightNum];

    // sample lights using MIS
}

Spectrum EstimateDirect(const HitRecord &it, const Light &light, const Scene &scene, Sampler &sampler, bool handleMedia, bool specular) {
    BxDFType bsdfFlags = specular ? BSDF_ALL :
                         BxDFType(BSDF_ALL & ~BSDF_SPECULAR);
    Spectrum Ld(0.f);
    Vector3f wi;
    float lightPdf = 0, scatteringPdf = 0;
    VisibilityTester visibility;
    Spectrum Li = light.Sample_Li(it, Point2f(), &wi, &lightPdf, &visibility);
    
    if (lightPdf > 0 && !Li.IsBlack()) {
        Spectrum f;
        if (it.IsSurface()) {
            const HitRecord &isect = (const HitRecord &)it;   
            f = isect.bsdf->f(isect.wo, wi, bsdfFlags) * AbsDot(wi, isect.normal);
            scatteringPdf = isect.bsdf->Pdf(isect.wo, wi, bsdfFlags);   
        }
        // TODO else if : medium case

        if (!f.IsBlack()) {
            if (!visibility.Unoccluded(scene))
                Li = Spectrum(0.f);
            // TODO else if(handleMedia)

            if (!Li.IsBlack()) {
                if (IsDelta(light.flags))
                    Ld += f * Li / lightPdf;
                // else compute weight
            }
        }
    }

    return Ld;
}