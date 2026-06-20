#include "libposture/geometry.h"

#include <cmath>

namespace libposture {

// Vector from `from` to `to`.
static Vec3 subtract(const Vec3& to, const Vec3& from) {
    return Vec3{to.x - from.x, to.y - from.y, to.z - from.z};
}

static float dot(const Vec3& u, const Vec3& v) {
    return u.x * v.x + u.y * v.y + u.z * v.z;
}

static float length(const Vec3& v) {
    return std::sqrt(dot(v, v));
}

float jointAngleDegrees(const Vec3& a, const Vec3& b, const Vec3& c) {
    // Two vectors starting at the middle joint `b`.
    Vec3 ba = subtract(a, b);
    Vec3 bc = subtract(c, b);

    float lengths = length(ba) * length(bc);
    if (lengths == 0.0f) {
        return 0.0f; // A point sits on top of `b`.
    }

    // cos(angle) = (ba . bc) / (|ba| * |bc|)
    float cosine = dot(ba, bc) / lengths;

    // Clamp so rounding error can't make acos() return NaN.
    if (cosine > 1.0f)  cosine = 1.0f;
    if (cosine < -1.0f) cosine = -1.0f;

    const float radiansToDegrees = 180.0f / 3.14159265f;
    return std::acos(cosine) * radiansToDegrees;
}

} // namespace libposture
