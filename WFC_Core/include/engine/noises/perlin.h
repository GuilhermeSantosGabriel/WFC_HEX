#ifndef PERLIN_H
#define PERLIN_H

#include <vector>

typedef struct {
    float x, y;
} Vector2;

class PerlinNoise {
private:
    unsigned int seed;

    Vector2 randomGradient(int ix, int iy);

    float dotGridGradient(int ix, int iy, float x, float y);

    float interpolate(float a0, float a1, float w);

public:
    PerlinNoise(unsigned int s) : seed(s) {}

    float sample(float x, float y);

    float normalized_perlin(float x, float y);
};

#endif