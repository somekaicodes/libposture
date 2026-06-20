#ifndef LIBPOSTURE_REP_COUNTER_H
#define LIBPOSTURE_REP_COUNTER_H

namespace libposture {

// Counts squat reps by watching the knee angle over time.
//
// A standing leg has a large knee angle; a squat bends it small. One rep is a
// full trip: bend below `downThreshold`, then straighten past `upThreshold`.
// The two separate thresholds (hysteresis) stop jitter from double-counting.
class RepCounter {
public:
    RepCounter(float downThreshold = 100.0f, float upThreshold = 160.0f);

    // Feed one smoothed knee angle (degrees). Returns the running rep count.
    int update(float kneeAngle);

    int count() const { return count_; }
    void reset();

private:
    enum class Phase { Standing, Squatting };

    float downThreshold_;
    float upThreshold_;
    Phase phase_ = Phase::Standing;
    int count_ = 0;
};

} // namespace libposture

#endif // LIBPOSTURE_REP_COUNTER_H
