#include "../core/medium.h"

class HomogeneousMedium : public Medium {
public:
    HomogeneousMedium(const Spectrum &sigma_a, const Spectrum &sigma_s, float g)
        : sigma_a(sigma_a), sigma_s(sigma_s), sigma_t(sigma_a + sigma_s), g(g) {}
    Spectrum Tr(const Ray &ray, Sampler &sampler) const;
    Spectrum Sample(const Ray &ray, Sampler &sampler, HitRecord &it) const;
private:
    const Spectrum sigma_a, sigma_s, sigma_t;
    const float g;
};