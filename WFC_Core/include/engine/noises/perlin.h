#ifndef PERLIN_H
#define PERLIN_H

#include "engine/noises/INoise.h"
#include <vector>

class PerlinNoise : public INoise {
public:
    struct Vector2 {
        float x, y;
    };

private:
    unsigned int seed;

    Vector2 randomGradient(int ix, int iy);

    float dotGridGradient(int ix, int iy, float x, float y);

    float interpolate(float a0, float a1, float w);

public:
    PerlinNoise(unsigned int s) : seed(s) {}

    float sample_implementation(float x, float y);
};

#endif