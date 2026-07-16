#include <gtest/gtest.h>
#include "MaxSNorm.h"
#include "ProbSumSNorm.h"

TEST(MaxSNormTest, ReturnsMaximum) {
    MaxSNorm s;
    EXPECT_DOUBLE_EQ(s.apply(0.3, 0.7), 0.7);
    EXPECT_DOUBLE_EQ(s.apply(0.9, 0.2), 0.9);
}
TEST(ProbabilisticSumSNormTest, ReturnsProbSum)
{
    ProbSumSNorm sNorm;
    EXPECT_DOUBLE_EQ(sNorm.apply(0.3, 0.5), 0.65);
    EXPECT_DOUBLE_EQ(sNorm.apply(0.4, 0.7), 0.82);
}
TEST(ProbabilisticSumSNormTest, ResultWithinBounds)
{
    ProbSumSNorm sNorm;

    double result = sNorm.apply(0.6, 0.8);

    EXPECT_GE(result, 0.0);
    EXPECT_LE(result, 1.0);
}