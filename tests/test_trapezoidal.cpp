#include <gtest/gtest.h>
#include "Trapezoidal.h"

TEST(TrapezoidalTest, Plateau) {
    Trapezoidal t("trap", 0, 2, 4, 6);

    EXPECT_DOUBLE_EQ(t.compute(3), 1.0);
}

TEST(TrapezoidalTest, OutsideRange) {
    Trapezoidal t("trap", 0, 2, 4, 6);

    EXPECT_DOUBLE_EQ(t.compute(-1), 0.0);
    EXPECT_DOUBLE_EQ(t.compute(7), 0.0);
}

TEST(TrapezoidalTest, RisingEdge) {
    Trapezoidal t("trap", 0, 2, 4, 6);

    EXPECT_DOUBLE_EQ(t.compute(1), 0.5);
}

TEST(TrapezoidalTest, FallingEdge) {
    Trapezoidal t("trap", 0, 2, 4, 6);

    EXPECT_DOUBLE_EQ(t.compute(5), 0.5);
}

TEST(TrapezoidalTest, NameStoredCorrectly) {
    Trapezoidal t("speed", 0, 2, 4, 6);

    EXPECT_EQ(t.getName(), "speed");
}