#include <Platform/TimeUtils.h>
#include <gtest/gtest.h>
#include <limits>

TEST(TimeMsTest, BasicArithmetic) {
    platform::TimeMs a{100};
    platform::TimeMs b{50};
    platform::TimeMs c = a + b;
    EXPECT_EQ(c.count(), 150u);
    EXPECT_EQ((c - a).count(), 50u);
}

TEST(TimeMsTest, RolloverSubtraction) {
    using rep = platform::TimeMs::rep;
    rep max = std::numeric_limits<rep>::max();
    platform::TimeMs near_wrap{max - 10};
    platform::TimeMs later{5};
    platform::TimeMs diff = later - near_wrap;
    EXPECT_EQ(diff.count(), 16u); // 5 - (max-10) wraps to 16
}

