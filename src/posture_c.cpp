#include "libposture/posture_c.h"

#include "libposture/squat_analyzer.h"

using libposture::SquatAnalyzer;
using libposture::Vec3;

// The opaque session simply owns a C++ analyzer.
struct PostureSession {
    SquatAnalyzer analyzer;
};

static Vec3 toVec3(const PostureVec3& v) {
    return Vec3{v.x, v.y, v.z};
}

PostureSession* posture_session_create(void) {
    return new PostureSession();
}

void posture_session_destroy(PostureSession* session) {
    delete session; // deleting nullptr is safe
}

void posture_session_reset(PostureSession* session) {
    if (session != nullptr) {
        session->analyzer.reset();
    }
}

PostureResult posture_session_update(PostureSession* session, PostureFrame frame) {
    PostureResult result = {};
    if (session == nullptr) {
        return result;
    }

    SquatAnalyzer::Frame in;
    in.timestamp = frame.timestamp;
    in.shoulder = toVec3(frame.shoulder);
    in.hip = toVec3(frame.hip);
    in.leftKnee = toVec3(frame.leftKnee);
    in.rightKnee = toVec3(frame.rightKnee);
    in.leftAnkle = toVec3(frame.leftAnkle);
    in.rightAnkle = toVec3(frame.rightAnkle);

    SquatAnalyzer::Result out = session->analyzer.update(in);

    result.repCount = out.repCount;
    result.kneeAngle = out.kneeAngle;
    result.repJustCompleted = out.repJustCompleted ? 1 : 0;
    result.insufficientDepth = out.lastRepForm.insufficientDepth ? 1 : 0;
    result.excessiveForwardLean = out.lastRepForm.excessiveForwardLean ? 1 : 0;
    result.kneesCavingIn = out.lastRepForm.kneesCavingIn ? 1 : 0;
    return result;
}
