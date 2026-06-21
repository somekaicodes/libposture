#include "libposture/squat_analyzer.h"

namespace libposture {

SquatAnalyzer::Result SquatAnalyzer::update(const Frame& frame) {
    // Knee angle from each leg, averaged for stability.
    float leftAngle = jointAngleDegrees(frame.leftHip, frame.leftKnee, frame.leftAnkle);
    float rightAngle = jointAngleDegrees(frame.rightHip, frame.rightKnee, frame.rightAnkle);
    float rawAngle = (leftAngle + rightAngle) * 0.5f;

    float smoothed = angleFilter_.filter(rawAngle, static_cast<float>(frame.timestamp));

    // Remember the deepest point so we can judge form at the bottom.
    if (smoothed < minKneeAngle_) {
        minKneeAngle_ = smoothed;
        bottomFrame_ = frame;
    }

    int before = repCounter_.count();
    int after = repCounter_.update(smoothed);

    Result result;
    result.kneeAngle = smoothed;
    result.repCount = after;
    result.repJustCompleted = (after > before);

    // When a rep finishes, grade the form from the deepest frame we saw.
    if (result.repJustCompleted) {
        SquatFrame squat;
        squat.minKneeAngle = minKneeAngle_;
        squat.shoulder = bottomFrame_.shoulder;
        // Torso lean uses the pelvis centre: the midpoint of the two hips.
        squat.hip = Vec3{
            (bottomFrame_.leftHip.x + bottomFrame_.rightHip.x) * 0.5f,
            (bottomFrame_.leftHip.y + bottomFrame_.rightHip.y) * 0.5f,
            (bottomFrame_.leftHip.z + bottomFrame_.rightHip.z) * 0.5f
        };
        squat.leftKnee = bottomFrame_.leftKnee;
        squat.rightKnee = bottomFrame_.rightKnee;
        squat.leftAnkle = bottomFrame_.leftAnkle;
        squat.rightAnkle = bottomFrame_.rightAnkle;
        lastRepForm_ = checkSquatForm(squat);

        minKneeAngle_ = 180.0f; // start fresh for the next rep
    }

    result.lastRepForm = lastRepForm_;
    return result;
}

void SquatAnalyzer::reset() {
    angleFilter_ = OneEuroFilter();
    repCounter_.reset();
    minKneeAngle_ = 180.0f;
    lastRepForm_ = FormFlags{};
}

} // namespace libposture
