#include <gtest/gtest.h>
#include "Consequent.h"

TEST(ConsequentTest, DefaultConstructor) {
    Consequent c;

    EXPECT_TRUE(c.getConditions().empty());
}

TEST(ConsequentTest, ConstructorStoresConditions) {
    std::vector<Condition> conditions = {
        {"Fan", "Fast"},
        {"Heater", "Off"}
    };

    Consequent c(conditions);

    EXPECT_EQ(c.getConditions().size(), 2);
}

TEST(ConsequentTest, AddCondition) {
    Consequent c;

    c.addCondition(Condition("Fan","Fast"));

    ASSERT_EQ(c.getConditions().size(), 1);

    EXPECT_EQ(c.getConditions()[0].variableName, "Fan");
    EXPECT_EQ(c.getConditions()[0].setName, "Fast");
}