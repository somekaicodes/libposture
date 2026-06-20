#include "libposture/form.h"

#include <gtest/gtest.h>

using libposture::checkSquatForm;
using libposture::FormFlags;
using libposture::SquatFrame;
using libposture::torsoLeanDegrees;
using libposture::Vec3;

// A shoulder straight above the hip means an upright torso (0 degrees).
TEST(FormTest, UprightTorsoHasNoLean) {
    EXPECT_NEAR(torsoLeanDegrees(Vec3{0, 1, 0}, Vec3{0, 0, 0}), 0.0f, 0.1f);
}

TEST(FormTest, TorsoLeanMeasures45Degrees) {
    EXPECT_NEAR(torsoLeanDegrees(Vec3{1, 1, 0}, Vec3{0, 0, 0}), 45.0f, 0.1f);
}

TEST(FormTest, TorsoLeanMeasures90Degrees) {
    EXPECT_NEAR(torsoLeanDegrees(Vec3{1, 0, 0}, Vec3{0, 0, 0}), 90.0f, 0.1f);
}

// A clean, deep, upright squat with knees over the feet: no flags.
static SquatFrame goodSquat() {
    SquatFrame frame;
    frame.minKneeAngle = 80.0f;
    frame.shoulder = Vec3{0.0f, 1.6f, 0.0f};
    frame.hip = Vec3{0.0f, 1.0f, 0.0f};
    frame.leftKnee = Vec3{-0.2f, 0.5f, 0.0f};
    frame.rightKnee = Vec3{0.2f, 0.5f, 0.0f};
    frame.leftAnkle = Vec3{-0.2f, 0.0f, 0.0f};
    frame.rightAnkle = Vec3{0.2f, 0.0f, 0.0f};
    return frame;
}

TEST(FormTest, GoodSquatHasNoFlags) {
    FormFlags flags = checkSquatForm(goodSquat());
    EXPECT_FALSE(flags.insufficientDepth);
    EXPECT_FALSE(flags.excessiveForwardLean);
    EXPECT_FALSE(flags.kneesCavingIn);
}

// Knee never bends past the target depth -> shallow squat.
TEST(FormTest, ShallowSquatFlagsDepth) {
    SquatFrame frame = goodSquat();
    frame.minKneeAngle = 110.0f;

    FormFlags flags = checkSquatForm(frame);
    EXPECT_TRUE(flags.insufficientDepth);
    EXPECT_FALSE(flags.excessiveForwardLean);
    EXPECT_FALSE(flags.kneesCavingIn);
}

// Torso folded well past the lean limit.
TEST(FormTest, LeaningTorsoFlagsForwardLean) {
    SquatFrame frame = goodSquat();
    frame.shoulder = Vec3{0.8f, 1.4f, 0.0f}; // ~63 degrees from vertical

    FormFlags flags = checkSquatForm(frame);
    EXPECT_TRUE(flags.excessiveForwardLean);
    EXPECT_FALSE(flags.insufficientDepth);
}

// Knees pulled much closer together than the ankles -> caving in.
TEST(FormTest, NarrowKneesFlagCaving) {
    SquatFrame frame = goodSquat();
    frame.leftKnee = Vec3{-0.05f, 0.5f, 0.0f};
    frame.rightKnee = Vec3{0.05f, 0.5f, 0.0f};

    FormFlags flags = checkSquatForm(frame);
    EXPECT_TRUE(flags.kneesCavingIn);
    EXPECT_FALSE(flags.insufficientDepth);
}
