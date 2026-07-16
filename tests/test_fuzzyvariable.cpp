#include <gtest/gtest.h>
#include "FuzzyVariable.h"
#include "Triangle.h"

TEST(FuzzyVariableTest, ConstructorStoresInfo) {
    FuzzyVariable var("Temperature", 0, 100);

    EXPECT_EQ(var.getName(), "Temperature");
    EXPECT_DOUBLE_EQ(var.getMinValue(), 0);
    EXPECT_DOUBLE_EQ(var.getMaxValue(), 100);
}

TEST(FuzzyVariableTest, InitiallyEmpty) {
    FuzzyVariable var("Temperature", 0, 100);

    EXPECT_TRUE(var.getSets().empty());
}

TEST(FuzzyVariableTest, AddSet) {
    FuzzyVariable var("Temperature", 0, 100);

    var.addSet(
        FuzzySet(
            "Hot",
            std::make_unique<Triangle>("Hot", 50, 75, 100)
        )
    );

    EXPECT_EQ(var.getSets().size(), 1);
}

TEST(FuzzyVariableTest, GetSetByName) {
    FuzzyVariable var("Temperature", 0, 100);

    var.addSet(
        FuzzySet(
            "Hot",
            std::make_unique<Triangle>("Hot", 50, 75, 100)
        )
    );

    const FuzzySet* set = var.getSet("Hot");

    ASSERT_NE(set, nullptr);
    EXPECT_EQ(set->getName(), "Hot");
}

TEST(FuzzyVariableTest, MissingSetReturnsNullptr) {
    FuzzyVariable var("Temperature", 0, 100);

    EXPECT_EQ(var.getSet("Cold"), nullptr);
}