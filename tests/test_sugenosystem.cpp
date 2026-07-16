#include <gtest/gtest.h>

#include "../include/SugenoSystem.h"

#include "../include/SugenoInference.h"
#include "../include/RuleEvaluator.h"

#include "../include/MinTNorm.h"
#include "../include/MaxSNorm.h"

#include "../include/InputVar.h"
#include "../include/InputValue.h"

#include "../include/Triangle.h"
#include "../include/FuzzySet.h"

#include "../include/SugenoRule.h"
#include "../include/ConstSugenoConsequent.h"
#include "../include/LinearSugenoConsequent.h"

#include <memory>
#include <unordered_map>


class SugenoSystemTest : public ::testing::Test
{
protected:

    std::shared_ptr<SugenoInference> engine;

    SugenoSystem system;


    void SetUp() override
    {
        engine =
            std::make_shared<SugenoInference>(
                RuleEvaluator(),
                std::make_shared<MinTNorm>(),
                std::make_shared<MaxSNorm>()
                );

        system.setInferenceEngine(
            engine
        );
    }


    InputVar createTemperature()
    {
        InputVar temperature(
            "temperature",
            0,
            100
        );


        temperature.addSet(
            FuzzySet(
                "cold",
                std::make_shared<Triangle>(
                    "cold",
                    0,
                    0,
                    70
                    )
            )
        );


        temperature.addSet(
            FuzzySet(
                "hot",
                std::make_shared<Triangle>(
                    "hot",
                    30,
                    100,
                    100
                    )
            )
        );


        return temperature;
    }


    SugenoRule constantRule(
        std::string setName,
        double output
    )
    {
        Antecedent ant(
            {
                Condition(
                    "temperature",
                    setName
                )
            }
        );


        return SugenoRule(
            ant,
            std::make_shared<ConstSugenoConsequent>(
                output
                )
        );
    }
};
TEST_F(
    SugenoSystemTest,
    DefaultConstructorHasNoRules
)
{
    SugenoSystem s;


    EXPECT_EQ(
        s.ruleCount(),
        0
    );
}



TEST_F(
    SugenoSystemTest,
    ConstructorStoresEngine
)
{
    SugenoSystem s(
        engine
    );


    EXPECT_EQ(
        s.getInferenceEngine(),
        engine
    );
}



TEST_F(
    SugenoSystemTest,
    SetInferenceEngineWorks
)
{
    SugenoSystem s;


    s.setInferenceEngine(
        engine
    );


    EXPECT_EQ(
        s.getInferenceEngine(),
        engine
    );
}

TEST_F(
    SugenoSystemTest,
    AddRuleIncreasesRuleCount
)
{
    system.addRule(
        constantRule(
            "cold",
            10
        )
    );


    EXPECT_EQ(
        system.ruleCount(),
        1
    );
}



TEST_F(
    SugenoSystemTest,
    RemoveRuleWorks
)
{
    system.addRule(
        constantRule(
            "cold",
            10
        )
    );


    system.removeRule(
        0
    );


    EXPECT_EQ(
        system.ruleCount(),
        0
    );
}



TEST_F(
    SugenoSystemTest,
    InvalidRemoveThrows
)
{
    EXPECT_THROW(
        system.removeRule(0),
        std::out_of_range
    );
}



TEST_F(
    SugenoSystemTest,
    ClearRulesWorks
)
{
    system.addRule(
        constantRule(
            "cold",
            10
        )
    );


    system.clearRules();


    EXPECT_TRUE(
        system.getRules().empty()
    );
}

TEST_F(
    SugenoSystemTest,
    ValidateThrowsWithoutEngine
)
{
    SugenoSystem s;


    s.addInputVar(
        createTemperature()
    );


    s.addRule(
        constantRule(
            "cold",
            10
        )
    );


    EXPECT_THROW(
        s.validate(),
        std::runtime_error
    );
}



TEST_F(
    SugenoSystemTest,
    ValidateThrowsWithoutRules
)
{
    system.addInputVar(
        createTemperature()
    );


    EXPECT_THROW(
        system.validate(),
        std::runtime_error
    );
}



TEST_F(
    SugenoSystemTest,
    ValidateSucceeds
)
{
    system.addInputVar(
        createTemperature()
    );


    system.addRule(
        constantRule(
            "cold",
            10
        )
    );


    EXPECT_NO_THROW(
        system.validate()
    );
}

TEST_F(
    SugenoSystemTest,
    SingleConstantRuleProducesValue
)
{
    system.addInputVar(
        createTemperature()
    );


    system.addRule(
        constantRule(
            "cold",
            20
        )
    );


    std::unordered_map<
        std::string,
        InputValue
    > inputs;


    inputs.emplace(
        "temperature",
        InputValue(25)
    );


    double result =
        system.evaluate(
            inputs
        );


    EXPECT_NEAR(
        result,
        20,
        1e-6
    );
}
TEST_F(
    SugenoSystemTest,
    MultipleRulesWeightedAverageWorks
)
{
    system.addInputVar(
        createTemperature()
    );


    system.addRule(
        constantRule(
            "cold",
            10
        )
    );


    system.addRule(
        constantRule(
            "hot",
            50
        )
    );


    std::unordered_map<
        std::string,
        InputValue
    > inputs;


    inputs.emplace(
        "temperature",
        InputValue(50)
    );


    double result =
        system.evaluate(
            inputs
        );


    EXPECT_GT(
        result,
        10
    );


    EXPECT_LT(
        result,
        50
    );
}

TEST_F(
    SugenoSystemTest,
    LinearConsequentEvaluationWorks
)
{
    system.addInputVar(
        createTemperature()
    );


    std::unordered_map<
        std::string,
        double
    > coeffs;


    coeffs.emplace(
        "temperature",
        2.0
    );


    auto consequent =
        std::make_shared<LinearSugenoConsequent>(
            coeffs,
            5
            );


    SugenoRule rule(
        Antecedent(
            {
                Condition(
                    "temperature",
                    "cold"
                )
            }
        ),
        consequent
    );


    system.addRule(
        rule
    );


    std::unordered_map<
        std::string,
        InputValue
    > inputs;


    inputs.emplace(
        "temperature",
        InputValue(20)
    );


    double result =
        system.evaluate(
            inputs
        );


    EXPECT_NEAR(
        result,
        45,
        1e-6
    );
}

TEST_F(
    SugenoSystemTest,
    NonCrispInputThrows
)
{
    system.addInputVar(
        createTemperature()
    );


    system.addRule(
        constantRule(
            "cold",
            10
        )
    );


    std::unordered_map<
        std::string,
        InputValue
    > inputs;


    inputs.emplace(
        "temperature",
        InputValue(
            std::string("cold")
        )
    );


    EXPECT_THROW(
        system.evaluate(
            inputs
        ),
        std::runtime_error
    );
}

TEST_F(
    SugenoSystemTest,
    ZeroFiringStrengthThrows
)
{
    system.addInputVar(
        createTemperature()
    );


    system.addRule(
        constantRule(
            "cold",
            10
        )
    );


    std::unordered_map<
        std::string,
        InputValue
    > inputs;


    inputs.emplace(
        "temperature",
        InputValue(100)
    );


    EXPECT_THROW(
        system.evaluate(
            inputs
        ),
        std::runtime_error
    );
}

TEST_F(
    SugenoSystemTest,
    RepeatedEvaluationSameResult
)
{
    system.addInputVar(
        createTemperature()
    );


    system.addRule(
        constantRule(
            "cold",
            30
        )
    );


    std::unordered_map<
        std::string,
        InputValue
    > inputs;


    inputs.emplace(
        "temperature",
        InputValue(20)
    );


    EXPECT_DOUBLE_EQ(
        system.evaluate(inputs),
        system.evaluate(inputs)
    );
}