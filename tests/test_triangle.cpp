#include <gtest/gtest.h>
#include "Triangle.h"
TEST(TriangleTest, PeakValue) {
	Triangle t("hot", 0, 5, 10);
	EXPECT_DOUBLE_EQ(t.compute(5), 1.0);
}
TEST(TriangleTest, LeftBoundary) {
	Triangle t("hot", 0, 5, 10);
	EXPECT_DOUBLE_EQ(t.compute(0), 0.0);
}
TEST(TraingleTest, RightBoundary) {
	Triangle t("hot", 0, 5, 10);
	EXPECT_DOUBLE_EQ(t.compute(10), 0.0);

}
TEST(TriangleTest, RisingEdge) {
	Triangle t("hot", 0, 5, 10);
	EXPECT_DOUBLE_EQ(t.compute(2.5), 0.5);
}
TEST(TriangleTest, FallingEdge) {
	Triangle t("hot", 0, 5, 10);
	EXPECT_DOUBLE_EQ(t.compute(7.5), 0.5);
}

TEST(TriangleTest, OutsideRange) {
	Triangle t("hot", 0, 5, 10);
	EXPECT_DOUBLE_EQ(t.compute(-1), 0.0);
	EXPECT_DOUBLE_EQ(t.compute(15), 0.0);
}

TEST(TriangleTest, NameReturnedCorrectly) {
	Triangle t("hot", 0, 5, 10);
	EXPECT_EQ(t.getName(), "hot");
}