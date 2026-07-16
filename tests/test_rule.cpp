#include <gtest/gtest.h>
#include "Rule.h"

TEST(RuleTest, ConstructorStoresAntecedentAndConsequent) {

    std::vector<Condition> antecedentConditions{
        Condition("Temperature", "Hot")
    };

    Antecedent antecedent(
        antecedentConditions,
        LogicalOperator::AND
    );

    std::vector<Condition> consequentConditions{
        Condition("Fan", "Fast")
    };

    Consequent consequent(consequentConditions);

    Rule rule(antecedent, consequent);

    EXPECT_EQ(rule.getAntecedent().getConditions().size(), 1);
    EXPECT_EQ(rule.getConsequent().getConditions().size(), 1);
}

TEST(RuleTest, AntecedentStoredCorrectly) {

    std::vector<Condition> antecedentConditions{
        Condition("Temperature", "Cold")
    };

    Antecedent antecedent(
        antecedentConditions,
        LogicalOperator::AND
    );

    std::vector<Condition> consequentConditions{
        Condition("Fan", "Slow")
    };

    Consequent consequent(consequentConditions);

    Rule rule(antecedent, consequent);

    EXPECT_EQ(
        rule.getAntecedent().getConditions()[0].setName,
        "Cold"
    );
}