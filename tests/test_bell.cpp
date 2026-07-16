#include <gtest/gtest.h>
#include "Bell.h"

TEST(BellTest, PeakAtCenter) {
    Bell b("bell", 2.0, 4.0, 0.0);

    EXPECT_DOUBLE_EQ(b.compute(0.0), 1.0);
}

TEST(BellTest, SymmetricAroundCenter) {
    Bell b("bell", 2.0, 4.0, 0.0);

    EXPECT_NEAR(b.compute(1.0), b.compute(-1.0), 1e-6);
}

TEST(BellTest, FarPointsHaveLowMembership) {
    Bell b("bell", 2.0, 4.0, 0.0);

    EXPECT_LT(b.compute(10.0), 0.1);
}

TEST(BellTest, NameStoredCorrectly) {
    Bell b("bellName", 2.0, 4.0, 0.0);

    EXPECT_EQ(b.getName(), "bellName");
}