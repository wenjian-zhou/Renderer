#include "../core/medium.h"

class GridDensityMedium : public Medium {
public:
    GridDensityMedium(const Spectrum &sigma_a, const Spectrum &sigma_s, 
                      float g, int nx, int ny, int nz, 
                      const Transform &mediumToWorld, const float *d)
                  : sigma_a(sigma_a), sigma_s(sigma_s), g(g), nx(nx), ny(ny), nz(nz),
                    WorldToMedium(Inverse(mediumToWorld)),
                    density(new float[nx * ny * nz]) {
                        memcpy((float *)density.get(), d, sizeof(float) * nx * ny * nz);
                        sigma_t = (sigma_a + sigma_s)[0];
                        float maxDensity = 0;
                        for (int i = 0; i < nx * ny * nz; ++i) {
                            maxDensity = std::max(maxDensity, density[i]);
                        }
                        invMaxDensity = 1.f / maxDensity;
                    }
    float Density(const Point3f &p) const;
    float D(const Point3i &p) const {
        Bounds3i sampleBounds(Point3i(0, 0, 0), Point3i(nx, ny, nz));
        if (!InsideExclusive(p, sampleBounds))
            return 0;
        return density[(p.z * ny + p.y) * nx + p.x];
    }
    Spectrum Sample(const Ray &ray, Sampler &sampler, MediumRecord *mi) const;
    Spectrum Tr(const Ray &ray, Sampler &sampler) const;
private:
    const Spectrum sigma_a, sigma_s;
    const float g;
    const int nx, ny, nz;
    const Transform WorldToMedium;
    std::unique_ptr<float[]> density;
    float sigma_t;
    float invMaxDensity;
};