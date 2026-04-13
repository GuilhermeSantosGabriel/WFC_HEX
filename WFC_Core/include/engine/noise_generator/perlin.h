#ifndef PERLIN_H
#define PERLIN_H

#include <vector>

struct Vector2 {
    float x, y;
};

float perlin(float x, float y);

float normalized_perlin(float x, float y, float frequency);

std::vector<std::vector<float>> perlin_noise_pixels(int width, int height);

#endif