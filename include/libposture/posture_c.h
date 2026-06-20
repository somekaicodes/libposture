#ifndef LIBPOSTURE_POSTURE_C_H
#define LIBPOSTURE_POSTURE_C_H

// Flat C interface to libposture, so Swift (Posture AI) can call in through a
// bridging header. Everything here is plain C: structs of floats and an
// opaque session handle.

#ifdef __cplusplus
extern "C" {
#endif

// A 3D point, matching an ARKit joint position.
typedef struct {
    float x;
    float y;
    float z;
} PostureVec3;

// One frame of joints needed to analyze a squat. `hip` is the pelvis centre.
typedef struct {
    double timestamp; // seconds
    PostureVec3 shoulder;
    PostureVec3 hip;
    PostureVec3 leftKnee;
    PostureVec3 rightKnee;
    PostureVec3 leftAnkle;
    PostureVec3 rightAnkle;
} PostureFrame;

// Result after one frame. Booleans use 0/1 to stay C-friendly.
typedef struct {
    int repCount;
    float kneeAngle;       // smoothed, degrees
    int repJustCompleted;  // 1 if a rep finished on this frame
    int insufficientDepth; // form of the most recent finished rep:
    int excessiveForwardLean;
    int kneesCavingIn;
} PostureResult;

// Opaque handle to an analysis session.
typedef struct PostureSession PostureSession;

// Create a session. Destroy it with posture_session_destroy when done.
PostureSession* posture_session_create(void);
void posture_session_destroy(PostureSession* session);

// Clear counts and history but keep the session alive.
void posture_session_reset(PostureSession* session);

// Feed one frame and get the current result.
PostureResult posture_session_update(PostureSession* session, PostureFrame frame);

#ifdef __cplusplus
}
#endif

#endif // LIBPOSTURE_POSTURE_C_H
