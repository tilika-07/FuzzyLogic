#include <gtest/gtest.h>
#include "Sigmoid.h"

TEST(SigmoidTest, MembershipAtCenter) {
    Sigmoid s("sig", 1.0, 0.0);

    EXPECT_NEAR(s.compute(0.0), 0.5, 1e-6);
}

TEST(SigmoidTest, IncreasingFunction) {
    Sigmoid s("sig", 1.0, 0.0);

    EXPECT_LT(s.compute(-1.0), s.compute(1.0));
}

TEST(SigmoidTest, ApproachesOne) {
    Sigmoid s("sig", 1.0, 0.0);

    EXPECT_GT(s.compute(10.0), 0.99);
}

TEST(SigmoidTest, NameStoredCorrectly) {
    Sigmoid s("sigmoidName", 1.0, 0.0);

    EXPECT_EQ(s.getName(), "sigmoidName");
}