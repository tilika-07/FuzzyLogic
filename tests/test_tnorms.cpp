#include <gtest/gtest.h>
#include "MinTNorm.h"
#include "ProdTNorm.h"
TEST(MinTNormTest, ReturnsMinimum) {
    MinTNorm t;
    EXPECT_DOUBLE_EQ(t.apply(0.3, 0.7), 0.3);
    EXPECT_DOUBLE_EQ(t.apply(1.0, 0.2), 0.2);
}

TEST(ProdTNormTest, ReturnsProduct) {
    ProdTNorm t;
    EXPECT_DOUBLE_EQ(t.apply(0.3, 0.7), 0.21);
    EXPECT_DOUBLE_EQ(t.apply(1.0, 0.2), 0.2);
}