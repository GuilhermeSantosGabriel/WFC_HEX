#include "engine/noises/perlin.h"

float ridged_multifractal(float x, float y) {
    float value = perlin(x,y);
    if (value < 0) value *= -1;
    return 1.0f - value;
}
