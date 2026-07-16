#include <gtest/gtest.h>
#include "FuzzySet.h"
#include "Triangle.h"
TEST(FuzzySetTest, MembershipDelegation)
{
	auto mf = std::make_unique<Triangle>("hot", 0, 5, 10);
	FuzzySet set("hot", std::move(mf));
	EXPECT_DOUBLE_EQ(set.membership(5), 1.0);
}
TEST(FuzzySetTest, CorrectName) {
	auto mf = std::make_unique<Triangle>("hot", 0, 5, 10);
	FuzzySet set("hot", std::move(mf));
	EXPECT_EQ(set.getName(), "hot");
}