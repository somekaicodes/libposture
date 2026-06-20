#ifndef LIBPOSTURE_GEOMETRY_H
#define LIBPOSTURE_GEOMETRY_H

namespace libposture {

// A point in 3D space, e.g. a body joint from ARKit.
struct Vec3 {
    float x;
    float y;
    float z;
};

// Angle in degrees at the middle joint `b`, formed by a-b-c.
// For a squat: a = hip, b = knee, c = ankle.
// Straight leg ~180; deep squat is much smaller. Returns 0 if degenerate.
float jointAngleDegrees(const Vec3& a, const Vec3& b, const Vec3& c);

} // namespace libposture

#endif // LIBPOSTURE_GEOMETRY_H
