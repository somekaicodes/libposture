#include "libposture/posture_c.h"

#include <gtest/gtest.h>

static PostureVec3 v(float x, float y, float z) {
    return PostureVec3{x, y, z};
}

static PostureFrame standing(double t) {
    PostureFrame f = {};
    f.timestamp = t;
    f.shoulder = v(0.0f, 2.6f, 0.0f);
    f.hip = v(0.0f, 2.0f, 0.0f);
    f.leftKnee = v(-0.2f, 1.0f, 0.0f);
    f.rightKnee = v(0.2f, 1.0f, 0.0f);
    f.leftAnkle = v(-0.2f, 0.0f, 0.0f);
    f.rightAnkle = v(0.2f, 0.0f, 0.0f);
    return f;
}

static PostureFrame bottom(double t) {
    PostureFrame f = {};
    f.timestamp = t;
    f.shoulder = v(0.0f, 1.6f, 0.0f);
    f.hip = v(0.0f, 1.0f, 0.0f);
    f.leftKnee = v(-0.2f, 0.0f, 0.0f);
    f.rightKnee = v(0.2f, 0.0f, 0.0f);
    f.leftAnkle = v(-0.2f, 0.1f, 0.9f);
    f.rightAnkle = v(0.2f, 0.1f, 0.9f);
    return f;
}

TEST(PostureCTest, CreateAndDestroy) {
    PostureSession* session = posture_session_create();
    ASSERT_NE(session, nullptr);
    posture_session_destroy(session);
}

// Drive a full squat through the C API and expect one rep.
TEST(PostureCTest, CountsOneRepThroughCApi) {
    PostureSession* session = posture_session_create();
    PostureResult result = {};
    double t = 0.0;

    for (int i = 0; i < 40; ++i) { result = posture_session_update(session, standing(t)); t += 1.0 / 60.0; }
    for (int i = 0; i < 40; ++i) { result = posture_session_update(session, bottom(t));   t += 1.0 / 60.0; }
    for (int i = 0; i < 40; ++i) { result = posture_session_update(session, standing(t)); t += 1.0 / 60.0; }

    EXPECT_EQ(result.repCount, 1);
    EXPECT_EQ(result.insufficientDepth, 0);
    posture_session_destroy(session);
}

// The API should not crash when handed a null session.
TEST(PostureCTest, NullSessionIsSafe) {
    PostureResult result = posture_session_update(nullptr, PostureFrame{});
    EXPECT_EQ(result.repCount, 0);

    posture_session_reset(nullptr);
    posture_session_destroy(nullptr);
}
