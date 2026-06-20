#include "libposture/geometry.h"

#include <gtest/gtest.h>

using libposture::jointAngleDegrees;
using libposture::Vec3;

// A straight leg (hip, knee, ankle in a line) should read ~180 degrees.
TEST(GeometryTest, StraightLineIsAbout180Degrees) {
    Vec3 hip{0.0f, 2.0f, 0.0f};
    Vec3 knee{0.0f, 1.0f, 0.0f};
    Vec3 ankle{0.0f, 0.0f, 0.0f};

    EXPECT_NEAR(jointAngleDegrees(hip, knee, ankle), 180.0f, 0.1f);
}

// A clean right angle should read ~90 degrees.
TEST(GeometryTest, RightAngleIsAbout90Degrees) {
    Vec3 a{0.0f, 1.0f, 0.0f};
    Vec3 b{0.0f, 0.0f, 0.0f};
    Vec3 c{1.0f, 0.0f, 0.0f};

    EXPECT_NEAR(jointAngleDegrees(a, b, c), 90.0f, 0.1f);
}

// A deep squat bends the knee well past 90 degrees, so the angle is acute.
TEST(GeometryTest, DeepSquatIsAcute) {
    Vec3 hip{1.0f, 1.0f, 0.0f};
    Vec3 knee{0.0f, 0.0f, 0.0f};
    Vec3 ankle{1.0f, -0.2f, 0.0f};

    float angle = jointAngleDegrees(hip, knee, ankle);
    EXPECT_GT(angle, 0.0f);
    EXPECT_LT(angle, 90.0f);
}

// If the points collapse onto each other there is no angle; expect 0.
TEST(GeometryTest, DegenerateInputReturnsZero) {
    Vec3 same{1.0f, 1.0f, 1.0f};

    EXPECT_FLOAT_EQ(jointAngleDegrees(same, same, same), 0.0f);
}
