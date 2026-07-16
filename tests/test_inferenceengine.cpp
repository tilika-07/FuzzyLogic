#include <gtest/gtest.h>

#include "../include/InferenceEngine.h"
#include "../include/MinTNorm.h"
#include "../include/MaxSNorm.h"
#include "../include/CentroidDefuzz.h"
#include "../include/InputVar.h"
#include "../include/FuzzyVariable.h"
#include "../include/Triangle.h"
#include "../include/Rule.h"

class InferenceEngineTest : public ::testing::Test
{
protected:
    std::shared_ptr<MinTNorm> minNorm =
        std::make_shared<MinTNorm>();

    std::shared_ptr<MaxSNorm> maxNorm =
        std::make_shared<MaxSNorm>();

    std::shared_ptr<CentroidDefuzz> centroid =
        std::make_shared<CentroidDefuzz>();

    InputVar temperature{ "Temperature",0,100 };
    FuzzyVariable fan{ "Fan",0,100 };

    std::unordered_map<
        std::string,
        std::shared_ptr<FuzzyVariable>> variables;

    void SetUp() override
    {
        temperature.addSet(
            FuzzySet(
                "Hot",
                std::make_shared<Triangle>(
                    "Hot",
                    40,
                    75,
                    100)));

        temperature.addSet(
            FuzzySet(
                "Cold",
                std::make_shared<Triangle>(
                    "Cold",
                    0,
                    25,
                    60)));

        fan.addSet(
            FuzzySet(
                "Fast",
                std::make_shared<Triangle>(
                    "Fast",
                    50,
                    75,
                    100)));

        fan.addSet(
            FuzzySet(
                "Slow",
                std::make_shared<Triangle>(
                    "Slow",
                    0,
                    25,
                    50)));

        variables["Temperature"] =
            std::make_shared<InputVar>(temperature);
    }

    Rule hotRule() const
    {
        return Rule(
            Antecedent(
                { Condition("Temperature","Hot") }
            ),
            Consequent(
                { Condition("Fan","Fast") }
        ));
    }

    Rule coldRule() const
    {
        return Rule(
            Antecedent(
                { Condition("Temperature","Cold") }
            ),
            Consequent(
                { Condition("Fan","Slow") }
        ));
    }
};
//checking missing constructor arguments
TEST_F(InferenceEngineTest, NullTNormThrows)
{
    EXPECT_THROW(
        InferenceEngine(
            nullptr,
            maxNorm,
            centroid),
        std::invalid_argument);
}

TEST_F(InferenceEngineTest, NullSNormThrows)
{
    EXPECT_THROW(
        InferenceEngine(
            minNorm,
            nullptr,
            centroid),
        std::invalid_argument);
}

TEST_F(InferenceEngineTest, NullDefuzzifierThrows)
{
    EXPECT_THROW(
        InferenceEngine(
            minNorm,
            maxNorm,
            nullptr),
        std::invalid_argument);
}

TEST_F(InferenceEngineTest, ValidConstructorDoesNotThrow)
{
    EXPECT_NO_THROW(
        InferenceEngine(
            minNorm,
            maxNorm,
            centroid));
}
TEST_F(InferenceEngineTest, SingleRuleProducesValidOutput)
{
    InferenceEngine engine(
        minNorm,
        maxNorm,
        centroid);

    std::unordered_map<std::string, InputValue> inputs{
        {"Temperature",InputValue(75.0)}
    };

    double result =
        engine.infer(
            { hotRule() },
            inputs,
            variables,
            fan);

    EXPECT_GE(result, fan.getMinValue());
    EXPECT_LE(result, fan.getMaxValue());
}

TEST_F(InferenceEngineTest, MultipleRulesProduceValidOutput)
{
    InferenceEngine engine(
        minNorm,
        maxNorm,
        centroid);

    std::unordered_map<std::string, InputValue> inputs{
        {"Temperature",InputValue(50.0)}
    };

    double result =
        engine.infer(
            { hotRule(),coldRule() },
            inputs,
            variables,
            fan);

    EXPECT_GE(result, fan.getMinValue());
    EXPECT_LE(result, fan.getMaxValue());
}
TEST_F(InferenceEngineTest, LinguisticInputInference)
{
    InferenceEngine engine(
        minNorm,
        maxNorm,
        centroid);

    std::unordered_map<std::string, InputValue> inputs{
        {"Temperature",InputValue("Hot")}
    };

    double result =
        engine.infer(
            { hotRule() },
            inputs,
            variables,
            fan);

    EXPECT_GE(result, 0);
    EXPECT_LE(result, 100);
}
TEST_F(InferenceEngineTest, FuzzyInputInference)
{
    InferenceEngine engine(
        minNorm,
        maxNorm,
        centroid);

    std::unordered_map<std::string, double> fuzzy{
        {"Hot",0.8}
    };

    std::unordered_map<std::string, InputValue> inputs{
        {"Temperature",InputValue(fuzzy)}
    };

    double result =
        engine.infer(
            { hotRule() },
            inputs,
            variables,
            fan);

    EXPECT_GE(result, 0);
    EXPECT_LE(result, 100);
}
TEST_F(InferenceEngineTest, MissingInputThrows)
{
    InferenceEngine engine(
        minNorm,
        maxNorm,
        centroid);

    EXPECT_THROW(
        engine.infer(
            { hotRule() },
            {},
            variables,
            fan),
        std::runtime_error);
}

TEST_F(InferenceEngineTest, UnknownVariableThrows)
{
    InferenceEngine engine(
        minNorm,
        maxNorm,
        centroid);

    Rule rule(
        Antecedent(
            { Condition("Pressure","High") }
        ),
        Consequent(
            { Condition("Fan","Fast") }
    ));

    std::unordered_map<std::string, InputValue> inputs{
        {"Pressure",InputValue(10.0)}
    };

    EXPECT_THROW(
        engine.infer(
            { rule },
            inputs,
            variables,
            fan),
        std::runtime_error);
}

TEST_F(InferenceEngineTest, UnknownFuzzySetThrows)
{
    InferenceEngine engine(
        minNorm,
        maxNorm,
        centroid);

    Rule rule(
        Antecedent(
            { Condition("Temperature","VeryHot") }
        ),
        Consequent(
            { Condition("Fan","Fast") }
    ));

    std::unordered_map<std::string, InputValue> inputs{
        {"Temperature",InputValue(75.0)}
    };

    EXPECT_THROW(
        engine.infer(
            { rule },
            inputs,
            variables,
            fan),
        std::runtime_error);
}

TEST_F(InferenceEngineTest, EmptyAntecedentThrows)
{
    InferenceEngine engine(
        minNorm,
        maxNorm,
        centroid);

    Rule rule(
        Antecedent(),
        Consequent(
            { Condition("Fan","Fast") }
    ));

    std::unordered_map<std::string, InputValue> inputs{
        {"Temperature",InputValue(75.0)}
    };

    EXPECT_THROW(
        engine.infer(
            { rule },
            inputs,
            variables,
            fan),
        std::runtime_error);
}

TEST_F(InferenceEngineTest, EmptyRuleBaseThrows)
{
    InferenceEngine engine(
        minNorm,
        maxNorm,
        centroid);

    std::unordered_map<std::string, InputValue> inputs{
        {"Temperature",InputValue(75.0)}
    };

    EXPECT_THROW(
        engine.infer(
            {},
            inputs,
            variables,
            fan),
        std::runtime_error);
}