#ifndef LIBPOSTURE_SQUAT_ANALYZER_H
#define LIBPOSTURE_SQUAT_ANALYZER_H

#include "libposture/filter.h"
#include "libposture/form.h"
#include "libposture/geometry.h"
#include "libposture/rep_counter.h"

namespace libposture {

// Ties the whole pipeline together: knee angle -> smoothing -> rep counting
// -> form check. Feed it one body-tracking frame at a time.
class SquatAnalyzer {
public:
    // One frame of 3D joints (from ARKit). `hip` is the pelvis centre.
    struct Frame {
        double timestamp = 0.0; // seconds
        Vec3 shoulder;
        Vec3 hip;
        Vec3 leftKnee;
        Vec3 rightKnee;
        Vec3 leftAnkle;
        Vec3 rightAnkle;
    };

    struct Result {
        int repCount = 0;
        float kneeAngle = 0.0f;  // smoothed, degrees
        bool repJustCompleted = false;
        FormFlags lastRepForm;   // form of the most recent finished rep
    };

    Result update(const Frame& frame);
    void reset();

private:
    OneEuroFilter angleFilter_;
    RepCounter repCounter_;

    float minKneeAngle_ = 180.0f; // deepest point seen in the current rep
    Frame bottomFrame_;           // joints at that deepest point
    FormFlags lastRepForm_;
};

} // namespace libposture

#endif // LIBPOSTURE_SQUAT_ANALYZER_H
