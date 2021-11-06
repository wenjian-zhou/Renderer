#ifndef SAMPLER_H
#define SAMPLER_H

#include "global.h"
#include "vector.h"

#include <random>

class Sampler {
public:
    Sampler() {}

    virtual void Setup(uint64_t s) {
        engine.seed(s);
    }

    virtual float Next1D() {
        return rng(engine);
    }

    virtual Vector2f Next2D() {
        return Vector2f(Next1D(), Next1D());
    }

private:
    std::default_random_engine engine;
    std::uniform_real_distribution<float> rng;
};

#endif