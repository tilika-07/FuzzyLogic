#include <gtest/gtest.h>
#include "SugenoRule.h"
#include "ConstSugenoConsequent.h"

TEST(SugenoRuleTest, StoresConsequent) {
    Antecedent ant(
        { {"Temperature", "Hot"} },
        LogicalOperator::AND
    );

    auto consequent =
        std::make_shared<ConstSugenoConsequent>(10.0);

    SugenoRule rule(ant, consequent);

    EXPECT_EQ(
        rule.getAntecedent().getConditions().size(),
        1
    );

    EXPECT_DOUBLE_EQ(
        rule.getConsequent().evaluate({}),
        10.0
    );
}