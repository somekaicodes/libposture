#include "libposture/rep_counter.h"

namespace libposture {

RepCounter::RepCounter(float downThreshold, float upThreshold)
    : downThreshold_(downThreshold), upThreshold_(upThreshold) {}

int RepCounter::update(float kneeAngle) {
    switch (phase_) {
        case Phase::Standing:
            // Bent down far enough: we're now in the squat.
            if (kneeAngle < downThreshold_) {
                phase_ = Phase::Squatting;
            }
            break;

        case Phase::Squatting:
            // Straightened back up: that completes one rep.
            if (kneeAngle > upThreshold_) {
                phase_ = Phase::Standing;
                count_ += 1;
            }
            break;
    }
    return count_;
}

void RepCounter::reset() {
    phase_ = Phase::Standing;
    count_ = 0;
}

} // namespace libposture
