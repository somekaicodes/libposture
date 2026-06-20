#include "libposture/form.h"

#include <cmath>

namespace libposture {

float torsoLeanDegrees(const Vec3& shoulder, const Vec3& hip) {
    // Vector pointing from the hip up to the shoulder.
    float dx = shoulder.x - hip.x;
    float dy = shoulder.y - hip.y;
    float dz = shoulder.z - hip.z;

    float length = std::sqrt(dx * dx + dy * dy + dz * dz);
    if (length == 0.0f) {
        return 0.0f;
    }

    // Compare against straight up (the +y axis): cos(angle) = dy / length.
    float cosine = dy / length;
    if (cosine > 1.0f)  cosine = 1.0f;
    if (cosine < -1.0f) cosine = -1.0f;

    const float radiansToDegrees = 180.0f / 3.14159265f;
    return std::acos(cosine) * radiansToDegrees;
}

FormFlags checkSquatForm(const SquatFrame& frame, const FormThresholds& thresholds) {
    FormFlags flags;

    // Depth: a larger minimum knee angle means the squat was too shallow.
    flags.insufficientDepth = frame.minKneeAngle > thresholds.targetDepthAngle;

    // Lean: torso tilted too far forward.
    float lean = torsoLeanDegrees(frame.shoulder, frame.hip);
    flags.excessiveForwardLean = lean > thresholds.maxTorsoLean;

    // Knees caving: knees much closer together than the ankles.
    float kneeWidth = std::fabs(frame.leftKnee.x - frame.rightKnee.x);
    float ankleWidth = std::fabs(frame.leftAnkle.x - frame.rightAnkle.x);
    if (ankleWidth > 0.0f) {
        flags.kneesCavingIn = (kneeWidth / ankleWidth) < thresholds.minKneeWidthRatio;
    }

    return flags;
}

} // namespace libposture
