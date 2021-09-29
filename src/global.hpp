#ifndef RENDERER_GLOBAL_H
#define RENDERER_GLOBAL_H

#include <math.h>

float SafeSqrt(const float &s)
{
    if (s < 0.f) return 0;
    else return sqrt(s);
}

#endif