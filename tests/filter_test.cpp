#include "libposture/filter.h"

#include <gtest/gtest.h>

using libposture::LowPassFilter;
using libposture::OneEuroFilter;

// The first sample has nothing to blend with, so it passes through.
TEST(LowPassFilterTest, FirstSampleIsUnchanged) {
    LowPassFilter filter;
    EXPECT_FLOAT_EQ(filter.filter(10.0f, 0.5f), 10.0f);
}

// alpha = 1 means "no smoothing": always return the newest value.
TEST(LowPassFilterTest, AlphaOneFollowsInput) {
    LowPassFilter filter;
    filter.filter(10.0f, 1.0f);
    EXPECT_FLOAT_EQ(filter.filter(20.0f, 1.0f), 20.0f);
}

// alpha = 0.5 blends halfway between the new value and the previous one.
TEST(LowPassFilterTest, AlphaHalfBlendsEvenly) {
    LowPassFilter filter;
    filter.filter(10.0f, 0.5f);                     // primes previous = 10
    EXPECT_FLOAT_EQ(filter.filter(20.0f, 0.5f), 15.0f);
}

// The first sample passes straight through here too.
TEST(OneEuroFilterTest, FirstSampleIsUnchanged) {
    OneEuroFilter filter;
    EXPECT_FLOAT_EQ(filter.filter(5.0f, 0.0f), 5.0f);
}

// A steady signal should stay put after smoothing.
TEST(OneEuroFilterTest, ConstantSignalStaysConstant) {
    OneEuroFilter filter;
    float value = 0.0f;
    for (int i = 0; i < 30; ++i) {
        value = filter.filter(5.0f, i / 60.0f);
    }
    EXPECT_NEAR(value, 5.0f, 0.01f);
}

// A jittery 0/10 signal should be smoothed to somewhere in between.
TEST(OneEuroFilterTest, ReducesJitter) {
    OneEuroFilter filter;
    float value = 0.0f;
    for (int i = 0; i < 30; ++i) {
        float noisy = (i % 2 == 0) ? 0.0f : 10.0f;
        value = filter.filter(noisy, i / 60.0f);
    }
    EXPECT_GT(value, 1.0f);
    EXPECT_LT(value, 9.0f);
}

// After a sustained change, the output should catch up to the new level.
TEST(OneEuroFilterTest, EventuallyTracksAStep) {
    OneEuroFilter filter;
    float value = 0.0f;
    for (int i = 0; i < 200; ++i) {
        float input = (i < 20) ? 0.0f : 100.0f;
        value = filter.filter(input, i / 60.0f);
    }
    EXPECT_NEAR(value, 100.0f, 1.0f);
}
