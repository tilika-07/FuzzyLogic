#include <gtest/gtest.h>
#include <cmath>
#include "Gaussian.h"

TEST(GaussianTest, PeakAtMean) {
    Gaussian g("gauss", 0.0, 1.0);

    EXPECT_DOUBLE_EQ(g.compute(0.0), 1.0);
}

TEST(GaussianTest, SymmetricAroundMean) {
    Gaussian g("gauss", 0.0, 1.0);

    EXPECT_NEAR(g.compute(1.0), g.compute(-1.0), 1e-6);
}

TEST(GaussianTest, FarFromMeanSmallMembership) {
    Gaussian g("gauss", 0.0, 1.0);

    EXPECT_LT(g.compute(5.0), 0.01);
}

TEST(GaussianTest, NameStoredCorrectly) {
    Gaussian g("gaussName", 0.0, 1.0);

    EXPECT_EQ(g.getName(), "gaussName");
}