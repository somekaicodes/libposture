#include "libposture/filter.h"

#include <cmath>

namespace libposture {

float LowPassFilter::filter(float value, float alpha) {
    if (!initialized_) {
        initialized_ = true;
        previous_ = value;
        return value;
    }

    previous_ = alpha * value + (1.0f - alpha) * previous_;
    return previous_;
}

// Turn a cutoff frequency and time step into a smoothing factor in [0, 1].
static float smoothingFactor(float dt, float cutoff) {
    const float twoPi = 2.0f * 3.14159265f;
    float tau = 1.0f / (twoPi * cutoff);
    return 1.0f / (1.0f + tau / dt);
}

OneEuroFilter::OneEuroFilter(float minCutoff, float beta)
    : minCutoff_(minCutoff), beta_(beta) {}

float OneEuroFilter::filter(float value, float timestamp) {
    // First sample: nothing to smooth against yet.
    if (!hasPrevious_) {
        hasPrevious_ = true;
        previousTimestamp_ = timestamp;
        previousFiltered_ = value;
        return value;
    }

    float dt = timestamp - previousTimestamp_;
    previousTimestamp_ = timestamp;
    if (dt <= 0.0f) {
        dt = 1.0f / 60.0f; // Guard against bad timestamps (assume ~60 fps).
    }

    // How fast the signal is changing, lightly smoothed (derivative at 1 Hz).
    float speed = (value - previousFiltered_) / dt;
    float smoothedSpeed = speedFilter_.filter(speed, smoothingFactor(dt, 1.0f));

    // Raise the cutoff when motion is fast, which cuts lag.
    float cutoff = minCutoff_ + beta_ * std::fabs(smoothedSpeed);

    float result = valueFilter_.filter(value, smoothingFactor(dt, cutoff));
    previousFiltered_ = result;
    return result;
}

} // namespace libposture
