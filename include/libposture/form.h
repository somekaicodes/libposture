#ifndef LIBPOSTURE_FORM_H
#define LIBPOSTURE_FORM_H

#include "libposture/geometry.h"

namespace libposture {

// What was wrong with a squat (all false means good form).
struct FormFlags {
    bool insufficientDepth = false;     // didn't squat low enough
    bool excessiveForwardLean = false;  // torso folded too far forward
    bool kneesCavingIn = false;         // knees collapsed inward (valgus)
};

// Tunable limits for what counts as good form.
struct FormThresholds {
    float targetDepthAngle = 90.0f;  // knee must bend to at most this (degrees)
    float maxTorsoLean = 45.0f;      // max torso tilt from vertical (degrees)
    float minKneeWidthRatio = 0.6f;  // knees-apart / ankles-apart must stay above this
};

// Key joints at the lowest point of a squat. Uses 3D points (from ARKit),
// so angles and widths aren't distorted by the camera angle.
struct SquatFrame {
    float minKneeAngle;  // smallest knee angle reached during the rep
    Vec3 shoulder;
    Vec3 hip;
    Vec3 leftKnee;
    Vec3 rightKnee;
    Vec3 leftAnkle;
    Vec3 rightAnkle;
};

// Tilt of the torso away from vertical, in degrees (0 = upright).
float torsoLeanDegrees(const Vec3& shoulder, const Vec3& hip);

// Check one squat against the thresholds and report any form problems.
FormFlags checkSquatForm(const SquatFrame& frame,
                         const FormThresholds& thresholds = FormThresholds{});

} // namespace libposture

#endif // LIBPOSTURE_FORM_H
