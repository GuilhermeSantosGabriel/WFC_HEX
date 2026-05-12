#ifndef RIDGED_MULTIFRACTAL_H
#define RIDGED_MULTIFRACTAL_H

#include "engine/noises/perlin.h"

class RidgedNoise {
private:
    unsigned int seed;

    PerlinNoise perlin_base;
public:
    RidgedNoise(unsigned int s) : seed(s), perlin_base(s) {}

    float sample(float x, float y);
};

#endif
