#include <gtest/gtest.h>
#include "InputValue.h"
TEST(InputValueTest, CrsipInput) {
	InputValue val(33);
	EXPECT_EQ(val.getType(), InputValue::Type::Crisp);
	EXPECT_DOUBLE_EQ(val.getCrispValue(), 33);
}
TEST(InputValueTest, LinguisticInput) {
	InputValue val("hot");
	EXPECT_EQ(val.getType(), InputValue::Type::Linguistic);
	EXPECT_EQ(val.getLinguisticValue(), "hot");
}
TEST(InputValueTest, FuzzyInput) {
    std::unordered_map<std::string, double> fuzzy = {
        {"Cold", 0.2},
        {"Warm", 0.7},
        {"Hot", 0.1}
    };
    InputValue value(fuzzy);
    EXPECT_EQ(value.getType(), InputValue::Type::Fuzzy);
    auto retrieved = value.getFuzzyValue();
    EXPECT_DOUBLE_EQ(retrieved.at("Warm"), 0.7);
}