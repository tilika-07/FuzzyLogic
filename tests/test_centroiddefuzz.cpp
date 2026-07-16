#include <gtest/gtest.h>
#include "CentroidDefuzz.h"

TEST(CentroidDefuzzTest, SymmetricDistribution) {
    CentroidDefuzz d;

    std::vector<double> x = { 0,1,2,3,4 };
    std::vector<double> mu = { 0,1,1,1,0 };

    EXPECT_NEAR(
        d.defuzzify(x, mu),
        2.0,
        1e-6
    );
}

TEST(CentroidDefuzzTest, SinglePeak) {
    CentroidDefuzz d;

    std::vector<double> x = { 0,1,2 };
    std::vector<double> mu = { 0,1,0 };

    EXPECT_NEAR(
        d.defuzzify(x, mu),
        1.0,
        1e-6
    );
}

TEST(CentroidDefuzzTest, EmptyVectorsThrow) {
    CentroidDefuzz d;

    EXPECT_THROW(
        d.defuzzify({}, {}),
        std::invalid_argument
    );
}

TEST(CentroidDefuzzTest, MismatchedSizesThrow) {
    CentroidDefuzz d;

    EXPECT_THROW(
        d.defuzzify({ 0,1 }, { 1 }),
        std::invalid_argument
    );
}

TEST(CentroidDefuzzTest, ZeroMembershipThrows) {
    CentroidDefuzz d;

    EXPECT_THROW(
        d.defuzzify(
            { 0,1,2 },
            { 0,0,0 }
        ),
        std::runtime_error
    );
}