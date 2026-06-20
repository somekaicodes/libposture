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

// Stand, drop into a squat, stand back up: that's one rep.
TEST(RepCounterTest, FullSquatCountsOne) {
    RepCounter counter;
    EXPECT_EQ(countReps(counter, {170.0f, 90.0f, 170.0f}), 1);
}

// A shallow dip that never passes the down threshold doesn't count.
TEST(RepCounterTest, ShallowDipDoesNotCount) {
    RepCounter counter;
    EXPECT_EQ(countReps(counter, {170.0f, 120.0f, 170.0f}), 0);
}

// Going down but not standing fully back up doesn't complete the rep.
TEST(RepCounterTest, IncompleteRiseDoesNotCount) {
    RepCounter counter;
    EXPECT_EQ(countReps(counter, {170.0f, 90.0f, 140.0f}), 0);
}

// Three clean reps in a row count as three.
TEST(RepCounterTest, CountsMultipleReps) {
    RepCounter counter;
    std::vector<float> angles = {
        170.0f, 90.0f, 170.0f,  // rep 1
        85.0f,  175.0f,         // rep 2
        95.0f,  165.0f,         // rep 3
    };
    EXPECT_EQ(countReps(counter, angles), 3);
}

// Jitter inside the dead zone (between the thresholds) must not double-count.
TEST(RepCounterTest, JitterBetweenThresholdsDoesNotDoubleCount) {
    RepCounter counter;
    std::vector<float> angles = {
        170.0f, 90.0f,                  // into the squat
        110.0f, 130.0f, 120.0f, 140.0f, // wobble, still below up threshold
        170.0f,                         // finally stand: exactly one rep
    };
    EXPECT_EQ(countReps(counter, angles), 1);
}

TEST(RepCounterTest, ResetClearsCount) {
    RepCounter counter;
    countReps(counter, {170.0f, 90.0f, 170.0f});
    counter.reset();
    EXPECT_EQ(counter.count(), 0);
}
