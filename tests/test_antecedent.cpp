#include <gtest/gtest.h>
#include "Antecedent.h"
TEST(AntecedentTest, DefaultOp) {
	Antecedent a;
	EXPECT_EQ(a.getOperator(), LogicalOperator::AND);
}
TEST(AntecedentTest, AddCondition) {
	Antecedent a;
	a.addCondition({ "Temperature","hot" });
	EXPECT_EQ(a.getConditions().size(), 1);
}
TEST(AntecedentTest, ConstructorCondition) {
	std::vector<Condition> conditions = {
	   Condition("Temperature", "Hot"),
	   Condition("Humidity", "High")
	};

	Antecedent a(conditions, LogicalOperator::OR);

	EXPECT_EQ(a.getConditions().size(), 2);
	EXPECT_EQ(a.getOperator(), LogicalOperator::OR);
}
