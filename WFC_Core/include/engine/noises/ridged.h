#ifndef RIDGED_MULTIFRACTAL_H
#define RIDGED_MULTIFRACTAL_H

#include "engine/noises/INoise.h"

class RidgedNoise  : public INoise {
private:
    INoise& base_noise;
public:
    RidgedNoise(INoise& b_n) : base_noise(b_n) {}

    float sample_implementation(float x, float y);
};

#endif
