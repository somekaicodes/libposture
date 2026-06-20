#ifndef LIBPOSTURE_FILTER_H
#define LIBPOSTURE_FILTER_H

namespace libposture {

// Exponential low-pass filter: blends each new value with the last one.
// alpha in [0, 1]: higher alpha follows the signal faster (less smoothing).
class LowPassFilter {
public:
    float filter(float value, float alpha);

private:
    bool initialized_ = false;
    float previous_ = 0.0f;
};

// One-Euro filter: smooths a noisy signal while keeping lag low.
// It eases off the smoothing when the signal moves fast, so quick squat
// motion stays responsive but a held pose stops jittering.
class OneEuroFilter {
public:
    // minCutoff: base smoothing (lower = smoother when slow or still).
    // beta:      how much fast motion reduces smoothing (higher = snappier).
    OneEuroFilter(float minCutoff = 1.0f, float beta = 0.0f);

    // value: raw sample. timestamp: seconds. Returns the smoothed value.
    float filter(float value, float timestamp);

private:
    float minCutoff_;
    float beta_;

    bool hasPrevious_ = false;
    float previousTimestamp_ = 0.0f;
    float previousFiltered_ = 0.0f;

    LowPassFilter valueFilter_;
    LowPassFilter speedFilter_;
};

} // namespace libposture

#endif // LIBPOSTURE_FILTER_H
