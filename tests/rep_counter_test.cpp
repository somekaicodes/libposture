#include "libposture/rep_counter.h"

#include <vector>

#include <gtest/gtest.h>

using libposture::RepCounter;

// Feed a whole sequence of angles and return the final count.
static int countReps(RepCounter& counter, const std::vector<float>& angles) {
    int count = 0;
    for (float angle : angles) {
        count = counter.update(angle);
    }
    return count;
}

TEST(RepCounterTest, StartsAtZero) {
    RepCounter counter;
    EXPECT_EQ(counter.count(), 0);
}

// Stand (~140), drop into a deep squat (~70), stand back up: that's one rep.
TEST(RepCounterTest, FullSquatCountsOne) {
    RepCounter counter;
    EXPECT_EQ(countReps(counter, {140.0f, 70.0f, 140.0f}), 1);
}

// A shallow dip that never passes the down threshold doesn't count.
TEST(RepCounterTest, ShallowDipDoesNotCount) {
    RepCounter counter;
    EXPECT_EQ(countReps(counter, {140.0f, 115.0f, 140.0f}), 0);
}

// Going down but not standing fully back up doesn't complete the rep.
TEST(RepCounterTest, IncompleteRiseDoesNotCount) {
    RepCounter counter;
    EXPECT_EQ(countReps(counter, {140.0f, 70.0f, 120.0f}), 0);
}

// Three clean reps in a row count as three.
TEST(RepCounterTest, CountsMultipleReps) {
    RepCounter counter;
    std::vector<float> angles = {
        140.0f, 70.0f, 140.0f,  // rep 1
        65.0f,  145.0f,         // rep 2
        72.0f,  135.0f,         // rep 3
    };
    EXPECT_EQ(countReps(counter, angles), 3);
}

// Jitter inside the dead zone (between the thresholds) must not double-count.
TEST(RepCounterTest, JitterBetweenThresholdsDoesNotDoubleCount) {
    RepCounter counter;
    std::vector<float> angles = {
        140.0f, 70.0f,                  // into the squat
        105.0f, 125.0f, 115.0f, 128.0f, // wobble, still below the up threshold
        140.0f,                         // finally stand: exactly one rep
    };
    EXPECT_EQ(countReps(counter, angles), 1);
}

TEST(RepCounterTest, ResetClearsCount) {
    RepCounter counter;
    countReps(counter, {140.0f, 70.0f, 140.0f});
    counter.reset();
    EXPECT_EQ(counter.count(), 0);
}
