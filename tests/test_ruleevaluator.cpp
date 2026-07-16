#include <gtest/gtest.h>
#include "../include/RuleEvaluator.h"
#include "../include/MinTNorm.h"
#include "../include/ProdTNorm.h"
#include "../include/SNorm.h"
#include "../include/MaxSNorm.h"
#include "../include/ProbSumSNorm.h"
#include "../include/InputVar.h"
#include "../include/FuzzyVariable.h"
#include "../include/Triangle.h"
#include "../include/Antecedent.h"
#include "../include/Rule.h"
class RuleEvaluatorTest : public ::testing::Test
{
protected:
    RuleEvaluator evaluator;

    MinTNorm minNorm;
    ProdTNorm productNorm;

    MaxSNorm maxNorm;
    ProbSumSNorm probSum;

    InputVar temperature{ "Temperature",0,100 };
    InputVar humidity{ "Humidity",0,100 };

    std::unordered_map<std::string, std::shared_ptr<FuzzyVariable>> variables;

    void SetUp() override
    {
        temperature.addSet(
            FuzzySet(
                "Hot",
                std::make_shared<Triangle>(
                    "Hot", 50, 75, 100)));

        humidity.addSet(
            FuzzySet(
                "High",
                std::make_shared<Triangle>(
                    "High", 50, 75, 100)));

        variables["Temperature"] =
            std::make_shared<InputVar>(temperature);

        variables["Humidity"] =
            std::make_shared<InputVar>(humidity);
    }
};
    TEST_F(RuleEvaluatorTest, CrispInputReturnsCorrectMembership) {
        Rule rule(Antecedent({ Condition("Temperature","Hot") }), Consequent());
        std::unordered_map<std::string, InputValue> inputs{
       {"Temperature",InputValue(75.0)}
        };

        EXPECT_DOUBLE_EQ(
            evaluator.evaluate(
                rule,
                inputs,
                variables,
                minNorm,
                maxNorm),
            1.0);

    }
    TEST_F(RuleEvaluatorTest, PartialMembershipComputedCorrectly)
    {
        Rule rule(
            Antecedent({ Condition("Temperature","Hot") }),
            Consequent());

        std::unordered_map<std::string, InputValue> inputs{
            {"Temperature",InputValue(62.5)}
        };

        EXPECT_NEAR(
            evaluator.evaluate(
                rule,
                inputs,
                variables,
                minNorm,
                maxNorm),
            0.5,
            1e-6);
    }
    TEST_F(RuleEvaluatorTest, LinguisticInputMatchesSet)
    {
        Rule rule(
            Antecedent({ Condition("Temperature","Hot") }),
            Consequent());

        std::unordered_map<std::string, InputValue> inputs{
            {"Temperature",InputValue("Hot")}
        };

        EXPECT_DOUBLE_EQ(
            evaluator.evaluate(
                rule,
                inputs,
                variables,
                minNorm,
                maxNorm),
            1.0);
    }
    TEST_F(RuleEvaluatorTest, LinguisticMismatchReturnsZero)
    {
        Rule rule(
            Antecedent({ Condition("Temperature","Hot") }),
            Consequent());

        std::unordered_map<std::string, InputValue> inputs{
            {"Temperature",InputValue("Cold")}
        };

        EXPECT_DOUBLE_EQ(
            evaluator.evaluate(
                rule,
                inputs,
                variables,
                minNorm,
                maxNorm),
            0.0);
    }
    TEST_F(RuleEvaluatorTest, AndUsesMinimumTNorm)
    {
        Antecedent ant(
            {
                Condition("Temperature","Hot"),
                Condition("Humidity","High")
            });

        Rule rule(ant, Consequent());

        std::unordered_map<std::string, InputValue> inputs{
            {"Temperature",InputValue(
                std::unordered_map<std::string,double>{ {"Hot",0.8}})},
            {"Humidity",InputValue(
                std::unordered_map<std::string,double>{ {"High",0.3}})}
        };

        EXPECT_DOUBLE_EQ(
            evaluator.evaluate(
                rule,
                inputs,
                variables,
                minNorm,
                maxNorm),
            0.3);
    }
    TEST_F(RuleEvaluatorTest, AndUsesProductTNorm)
    {
        Antecedent ant(
            {
                Condition("Temperature","Hot"),
                Condition("Humidity","High")
            });

        Rule rule(ant, Consequent());

        std::unordered_map<std::string, InputValue> inputs{
            {"Temperature",InputValue(
                std::unordered_map<std::string,double>{ {"Hot",0.8}})},
            {"Humidity",InputValue(
                std::unordered_map<std::string,double>{ {"High",0.3}})}
        };

        EXPECT_DOUBLE_EQ(
            evaluator.evaluate(
                rule,
                inputs,
                variables,
                productNorm,
                maxNorm),
            0.24);
    }
    TEST_F(RuleEvaluatorTest, OrUsesMaximumSNorm)
    {
        Antecedent ant(
            {
                Condition("Temperature","Hot"),
                Condition("Humidity","High")
            },
            LogicalOperator::OR);

        Rule rule(ant, Consequent());

        std::unordered_map<std::string, InputValue> inputs{
            {"Temperature",InputValue(
                std::unordered_map<std::string,double>{ {"Hot",0.4}})},
            {"Humidity",InputValue(
                std::unordered_map<std::string,double>{ {"High",0.9}})}
        };

        EXPECT_DOUBLE_EQ(
            evaluator.evaluate(
                rule,
                inputs,
                variables,
                minNorm,
                maxNorm),
            0.9);
    }
    TEST_F(RuleEvaluatorTest, OrUsesProbabilisticSum)
    {
        Antecedent ant(
            {
                Condition("Temperature","Hot"),
                Condition("Humidity","High")
            },
            LogicalOperator::OR);

        Rule rule(ant, Consequent());

        std::unordered_map<std::string, InputValue> inputs{
            {"Temperature",InputValue(
                std::unordered_map<std::string,double>{ {"Hot",0.4}})},
            {"Humidity",InputValue(
                std::unordered_map<std::string,double>{ {"High",0.8}})}
        };

        EXPECT_NEAR(
            evaluator.evaluate(
                rule,
                inputs,
                variables,
                minNorm,
                probSum),
            0.88,
            1e-6);
    }
    TEST_F(RuleEvaluatorTest, ThreeConditionAndUsesMinimumTNorm)
    {
        InputVar speed("Speed", 0, 100);

        speed.addSet(
            FuzzySet(
                "Fast",
                std::make_shared<Triangle>(
                    "Fast", 50, 75, 100)));

        variables["Speed"] =
            std::make_shared<InputVar>(speed);

        Antecedent ant(
            {
                Condition("Temperature","Hot"),
                Condition("Humidity","High"),
                Condition("Speed","Fast")
            });

        Rule rule(ant, Consequent());

        std::unordered_map<std::string, InputValue> inputs{
            {"Temperature",InputValue(
                std::unordered_map<std::string,double>{ {"Hot",0.8}})},
            {"Humidity",InputValue(
                std::unordered_map<std::string,double>{ {"High",0.6}})},
            {"Speed",InputValue(
                std::unordered_map<std::string,double>{ {"Fast",0.9}})}
        };

        EXPECT_DOUBLE_EQ(
            evaluator.evaluate(
                rule,
                inputs,
                variables,
                minNorm,
                maxNorm),
            0.6);
    }
    TEST_F(RuleEvaluatorTest, ThreeConditionAndUsesProductTNorm)
    {
        InputVar speed("Speed", 0, 100);

        speed.addSet(
            FuzzySet(
                "Fast",
                std::make_shared<Triangle>(
                    "Fast", 50, 75, 100)));

        variables["Speed"] =
            std::make_shared<InputVar>(speed);

        Antecedent ant(
            {
                Condition("Temperature","Hot"),
                Condition("Humidity","High"),
                Condition("Speed","Fast")
            });

        Rule rule(ant, Consequent());

        std::unordered_map<std::string, InputValue> inputs{
            {"Temperature",InputValue(
                std::unordered_map<std::string,double>{ {"Hot",0.8}})},
            {"Humidity",InputValue(
                std::unordered_map<std::string,double>{ {"High",0.6}})},
            {"Speed",InputValue(
                std::unordered_map<std::string,double>{ {"Fast",0.9}})}
        };

        EXPECT_NEAR(
            evaluator.evaluate(
                rule,
                inputs,
                variables,
                productNorm,
                maxNorm),
            0.432,
            1e-6);
    }
    TEST_F(RuleEvaluatorTest, ThreeConditionOrUsesMaximumSNorm)
    {
        InputVar speed("Speed", 0, 100);

        speed.addSet(
            FuzzySet(
                "Fast",
                std::make_shared<Triangle>(
                    "Fast", 50, 75, 100)));

        variables["Speed"] =
            std::make_shared<InputVar>(speed);

        Antecedent ant(
            {
                Condition("Temperature","Hot"),
                Condition("Humidity","High"),
                Condition("Speed","Fast")
            },
            LogicalOperator::OR);

        Rule rule(ant, Consequent());

        std::unordered_map<std::string, InputValue> inputs{
            {"Temperature",InputValue(
                std::unordered_map<std::string,double>{ {"Hot",0.3}})},
            {"Humidity",InputValue(
                std::unordered_map<std::string,double>{ {"High",0.7}})},
            {"Speed",InputValue(
                std::unordered_map<std::string,double>{ {"Fast",0.5}})}
        };

        EXPECT_DOUBLE_EQ(
            evaluator.evaluate(
                rule,
                inputs,
                variables,
                minNorm,
                maxNorm),
            0.7);
    }
    TEST_F(RuleEvaluatorTest, MixedInputTypesEvaluateCorrectly)
    {
        Antecedent ant(
            {
                Condition("Temperature","Hot"),
                Condition("Humidity","High")
            });

        Rule rule(ant, Consequent());

        std::unordered_map<std::string, InputValue> inputs{
            {"Temperature",InputValue("Hot")},
            {"Humidity",InputValue(
                std::unordered_map<std::string,double>{ {"High",0.4}})}
        };

        EXPECT_DOUBLE_EQ(
            evaluator.evaluate(
                rule,
                inputs,
                variables,
                minNorm,
                maxNorm),
            0.4);
    }
    TEST_F(RuleEvaluatorTest, CrispAndLinguisticInputsMatchAtPeak)
    {
        Rule rule(
            Antecedent({ Condition("Temperature","Hot") }),
            Consequent());

        std::unordered_map<std::string, InputValue> crisp{
            {"Temperature",InputValue(75.0)}
        };

        std::unordered_map<std::string, InputValue> linguistic{
            {"Temperature",InputValue("Hot")}
        };

        EXPECT_DOUBLE_EQ(
            evaluator.evaluate(
                rule,
                crisp,
                variables,
                minNorm,
                maxNorm),

            evaluator.evaluate(
                rule,
                linguistic,
                variables,
                minNorm,
                maxNorm));
    }
    TEST_F(RuleEvaluatorTest, MissingMembershipDefaultsToZero)
    {
        Rule rule(
            Antecedent({ Condition("Temperature","Hot") }),
            Consequent());

        std::unordered_map<std::string, double> fuzzy{
            {"Cold",0.9}
        };

        std::unordered_map<std::string, InputValue> inputs{
            {"Temperature",InputValue(fuzzy)}
        };

        EXPECT_DOUBLE_EQ(
            evaluator.evaluate(
                rule,
                inputs,
                variables,
                minNorm,
                maxNorm),
            0.0);
    }
    TEST_F(RuleEvaluatorTest, CorrectMembershipChosenFromFuzzyMap)
    {
        Rule rule(
            Antecedent({ Condition("Temperature","Hot") }),
            Consequent());

        std::unordered_map<std::string, double> fuzzy{
            {"Cold",0.2},
            {"Warm",0.5},
            {"Hot",0.9}
        };

        std::unordered_map<std::string, InputValue> inputs{
            {"Temperature",InputValue(fuzzy)}
        };

        EXPECT_DOUBLE_EQ(
            evaluator.evaluate(
                rule,
                inputs,
                variables,
                minNorm,
                maxNorm),
            0.9);
    }
    TEST_F(RuleEvaluatorTest, EvaluateMatchesEvaluateAntecedent)
    {
        Rule rule(
            Antecedent({ Condition("Temperature","Hot") }),
            Consequent());

        std::unordered_map<std::string, InputValue> inputs{
            {"Temperature",InputValue("Hot")}
        };

        EXPECT_DOUBLE_EQ(
            evaluator.evaluate(
                rule,
                inputs,
                variables,
                minNorm,
                maxNorm),

            evaluator.evaluateAntecedent(
                rule.getAntecedent(),
                inputs,
                variables,
                minNorm,
                maxNorm));
    }
    TEST_F(RuleEvaluatorTest, MissingInputVariableThrows)
    {
        Rule rule(
            Antecedent({ Condition("Temperature","Hot") }),
            Consequent());

        std::unordered_map<std::string, InputValue> inputs;

        EXPECT_THROW(
            evaluator.evaluate(
                rule,
                inputs,
                variables,
                minNorm,
                maxNorm),
            std::runtime_error);
    }
    TEST_F(RuleEvaluatorTest, UnknownVariableThrows)
    {
        Rule rule(
            Antecedent({ Condition("Pressure","High") }),
            Consequent());

        std::unordered_map<std::string, InputValue> inputs{
            {"Pressure",InputValue("High")}
        };

        EXPECT_THROW(
            evaluator.evaluate(
                rule,
                inputs,
                variables,
                minNorm,
                maxNorm),
            std::runtime_error);
    }
    TEST_F(RuleEvaluatorTest, UnknownFuzzySetThrows)
    {
        Rule rule(
            Antecedent({ Condition("Temperature","VeryHot") }),
            Consequent());

        std::unordered_map<std::string, InputValue> inputs{
            {"Temperature",InputValue(75.0)}
        };

        EXPECT_THROW(
            evaluator.evaluate(
                rule,
                inputs,
                variables,
                minNorm,
                maxNorm),
            std::runtime_error);
    }