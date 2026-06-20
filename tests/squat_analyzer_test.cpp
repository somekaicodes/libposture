#include "libposture/squat_analyzer.h"

#include <gtest/gtest.h>

using libposture::SquatAnalyzer;
using libposture::Vec3;

// Tall, straight-legged stance: knee angle reads high (above the up threshold).
static SquatAnalyzer::Frame standingFrame(double t) {
    SquatAnalyzer::Frame f;
    f.timestamp = t;
    f.shoulder = Vec3{0.0f, 2.6f, 0.0f};
    f.hip = Vec3{0.0f, 2.0f, 0.0f};
    f.leftKnee = Vec3{-0.2f, 1.0f, 0.0f};
    f.rightKnee = Vec3{0.2f, 1.0f, 0.0f};
    f.leftAnkle = Vec3{-0.2f, 0.0f, 0.0f};
    f.rightAnkle = Vec3{0.2f, 0.0f, 0.0f};
    return f;
}

// Deep, upright squat with knees over the feet: low knee angle, good form.
static SquatAnalyzer::Frame bottomFrame(double t) {
    SquatAnalyzer::Frame f;
    f.timestamp = t;
    f.shoulder = Vec3{0.0f, 1.6f, 0.0f};
    f.hip = Vec3{0.0f, 1.0f, 0.0f};
    f.leftKnee = Vec3{-0.2f, 0.0f, 0.0f};
    f.rightKnee = Vec3{0.2f, 0.0f, 0.0f};
    f.leftAnkle = Vec3{-0.2f, 0.1f, 0.9f};
    f.rightAnkle = Vec3{0.2f, 0.1f, 0.9f};
    return f;
}

TEST(SquatAnalyzerTest, StartsAtZeroReps) {
    SquatAnalyzer analyzer;
    SquatAnalyzer::Result result = analyzer.update(standingFrame(0.0));
    EXPECT_EQ(result.repCount, 0);
}

// Stand, hold a deep squat, stand again: that's one rep with clean form.
TEST(SquatAnalyzerTest, FullSquatCountsOneRepWithGoodForm) {
    SquatAnalyzer analyzer;
    SquatAnalyzer::Result result;
    bool sawCompletion = false;
    double t = 0.0;

    // Enough frames per phase to let the smoothing filter settle.
    for (int i = 0; i < 40; ++i) { result = analyzer.update(standingFrame(t)); t += 1.0 / 60.0; }
    for (int i = 0; i < 40; ++i) { result = analyzer.update(bottomFrame(t));   t += 1.0 / 60.0; }
    for (int i = 0; i < 40; ++i) {
        result = analyzer.update(standingFrame(t));
        if (result.repJustCompleted) sawCompletion = true;
        t += 1.0 / 60.0;
    }

    EXPECT_EQ(result.repCount, 1);
    EXPECT_TRUE(sawCompletion);
    EXPECT_FALSE(result.lastRepForm.insufficientDepth);
    EXPECT_FALSE(result.lastRepForm.excessiveForwardLean);
    EXPECT_FALSE(result.lastRepForm.kneesCavingIn);
}

TEST(SquatAnalyzerTest, ResetClearsReps) {
    SquatAnalyzer analyzer;
    double t = 0.0;
    for (int i = 0; i < 40; ++i) { analyzer.update(standingFrame(t)); t += 1.0 / 60.0; }
    for (int i = 0; i < 40; ++i) { analyzer.update(bottomFrame(t));   t += 1.0 / 60.0; }
    for (int i = 0; i < 40; ++i) { analyzer.update(standingFrame(t)); t += 1.0 / 60.0; }

    analyzer.reset();

    SquatAnalyzer::Result result = analyzer.update(standingFrame(t));
    EXPECT_EQ(result.repCount, 0);
}
