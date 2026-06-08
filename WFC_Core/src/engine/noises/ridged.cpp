#include "engine/noises/ridged.h"

float RidgedNoise::sample_implementation(float x, float y) {
    float value = this->base_noise.sample(x, y);
    if (value < 0) value *= -1;
    return 1.0f - value;
}
