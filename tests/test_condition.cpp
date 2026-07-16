#include <gtest/gtest.h>
#include "Condition.h"

TEST(ConditionTest, ConstructorStoresValues) {
    Condition c("Temperature", "Hot");

    EXPECT_EQ(c.variableName, "Temperature");
    EXPECT_EQ(c.setName, "Hot");
}

TEST(ConditionTest, DefaultConstructor) {
    Condition c;

    EXPECT_TRUE(c.variableName.empty());
    EXPECT_TRUE(c.setName.empty());
}