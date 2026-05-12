#include "engine/noises/ridged.h"

float RidgedNoise::sample(float x, float y) {
    float value = this->perlin_base.sample(x,y);
    if (value < 0) value *= -1;
    return 1.0f - value;
}
