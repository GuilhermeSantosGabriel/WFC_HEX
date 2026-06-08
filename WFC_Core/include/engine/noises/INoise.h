#ifndef I_NOISE_H
#define I_NOISE_H

#include <algorithm>
#include <cassert>

class INoise {
public:
    virtual ~INoise() = default;

    // Ensures [-1, 1] float return
    float sample(float x, float y) {
        float raw_value = sample_implementation(x, y);
        return std::clamp(raw_value, -1.0f, 1.0f);
    }
    
    // Ensures [0, 1] float return
    float sample_01(float x, float y) {
        return (sample(x, y) + 1.0f) * 0.5f;
    }

protected:
    // Specific Noise sample implementation
    virtual float sample_implementation(float x, float y) = 0;
};

#endif
