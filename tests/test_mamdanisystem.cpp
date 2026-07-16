#include <gtest/gtest.h>
#include "../include/MamdaniSystem.h"
#include "../include/MinTNorm.h"
#include "../include/MaxSNorm.h"
#include "../include/CentroidDefuzz.h"
#include "../include/InputVar.h"
#include "../include/OutputVar.h"
#include "../include/InputValue.h"
#include "../include/Triangle.h"
#include "../include/FuzzySet.h"
#include "../include/Antecedent.h"
#include "../include/Consequent.h"
#include "../include/Rule.h"
#include "../include/ProdTNorm.h"
#include <memory>
#include <unordered_map>


class MamdaniSystemTest : public ::testing::Test
{
protected:

    std::shared_ptr<InferenceEngine> engine;

    MamdaniSystem system;

    void SetUp() override
    {
        engine =
            std::make_shared<InferenceEngine>(
                std::make_shared<MinTNorm>(),
                std::make_shared<MaxSNorm>(),
                std::make_shared<CentroidDefuzz>()
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


    OutputVar createFan()
    {
        OutputVar fan(
            "fan",
            0,
            100
        );

        fan.addSet(
            FuzzySet(
                "slow",
                std::make_shared<Triangle>(
                    "slow",
                    0,
                    0,
                    50
                    )
            )
        );

        fan.addSet(
            FuzzySet(
                "fast",
                std::make_shared<Triangle>(
                    "fast",
                    50,
                    100,
                    100
                    )
            )
        );

        return fan;
    }


    Rule createColdRule()
    {
        Antecedent ant(
            {
                Condition(
                    "temperature",
                    "cold"
                )
            }
        );

        Consequent cons(
            {
                Condition(
                    "fan",
                    "slow"
                )
            }
        );

        return Rule(
            ant,
            cons
        );
    }
};


TEST_F(
    MamdaniSystemTest,
    DefaultConstructorHasNoRules
)
{
    MamdaniSystem empty;

    EXPECT_EQ(
        empty.ruleCount(),
        0
    );
}


TEST_F(
    MamdaniSystemTest,
    ConstructorStoresEngine
)
{
    MamdaniSystem s(
        engine
    );

    EXPECT_EQ(
        s.getInferenceEngine(),
        engine
    );
}


TEST_F(
    MamdaniSystemTest,
    SetInferenceEngineWorks
)
{
    MamdaniSystem s;

    s.setInferenceEngine(
        engine
    );

    EXPECT_EQ(
        s.getInferenceEngine(),
        engine
    );
}

TEST_F(
    MamdaniSystemTest,
    AddRuleIncreasesRuleCount
)
{
    system.addRule(
        createColdRule()
    );

    EXPECT_EQ(
        system.ruleCount(),
        1
    );
}


TEST_F(
    MamdaniSystemTest,
    StoresAddedRules
)
{
    system.addRule(
        createColdRule()
    );

    EXPECT_EQ(
        system.getRules().size(),
        1
    );
}


TEST_F(
    MamdaniSystemTest,
    RemoveRuleWorks
)
{
    system.addRule(
        createColdRule()
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
    MamdaniSystemTest,
    RemovingInvalidRuleThrows
)
{
    EXPECT_THROW(
        system.removeRule(0),
        std::out_of_range
    );
}


TEST_F(
    MamdaniSystemTest,
    ClearRulesWorks
)
{
    system.addRule(
        createColdRule()
    );

    system.clearRules();

    EXPECT_TRUE(
        system.getRules().empty()
    );
}


TEST_F(
    MamdaniSystemTest,
    ValidateThrowsWithoutVariables
)
{
    system.addRule(
        createColdRule()
    );

    EXPECT_THROW(
        system.validate(),
        std::runtime_error
    );
}


TEST_F(
    MamdaniSystemTest,
    ValidateThrowsWithoutEngine
)
{
    MamdaniSystem s;

    s.addInputVar(
        createTemperature()
    );

    s.addOutputVar(
        createFan()
    );

    s.addRule(
        createColdRule()
    );

    EXPECT_THROW(
        s.validate(),
        std::runtime_error
    );
}


TEST_F(
    MamdaniSystemTest,
    ValidateThrowsWithoutRules
)
{
    system.addInputVar(
        createTemperature()
    );

    system.addOutputVar(
        createFan()
    );

    EXPECT_THROW(
        system.validate(),
        std::runtime_error
    );
}


TEST_F(
    MamdaniSystemTest,
    ValidateSuccessfulSystem
)
{
    system.addInputVar(
        createTemperature()
    );

    system.addOutputVar(
        createFan()
    );

    system.addRule(
        createColdRule()
    );

    EXPECT_NO_THROW(
        system.validate()
    );
}


// ----------------------------------------------------
// Evaluation tests
// ----------------------------------------------------

TEST_F(
    MamdaniSystemTest,
    EvaluateSingleRuleProducesOutput
)
{
    system.addInputVar(
        createTemperature()
    );

    system.addOutputVar(
        createFan()
    );

    system.addRule(
        createColdRule()
    );

    std::unordered_map<
        std::string,
        InputValue
    > inputs;

    inputs.emplace(
        "temperature",
        InputValue(25.0)
    );

    double result =
        system.evaluate(
            inputs,
            "fan"
        );

    EXPECT_GE(
        result,
        0
    );

    EXPECT_LE(
        result,
        100
    );
}


TEST_F(
    MamdaniSystemTest,
    UnknownOutputThrows
)
{
    system.addInputVar(
        createTemperature()
    );

    system.addOutputVar(
        createFan()
    );

    system.addRule(
        createColdRule()
    );

    std::unordered_map<
        std::string,
        InputValue
    > inputs;

    inputs.emplace(
        "temperature",
        InputValue(25)
    );


    EXPECT_THROW(
        system.evaluate(
            inputs,
            "unknown"
        ),
        std::runtime_error
    );
}



TEST_F(
    MamdaniSystemTest,
    RepeatedEvaluationProducesSameResult
)
{
    system.addInputVar(
        createTemperature()
    );

    system.addOutputVar(
        createFan()
    );

    system.addRule(
        createColdRule()
    );

    std::unordered_map<
        std::string,
        InputValue
    > inputs;

    inputs.emplace(
        "temperature",
        InputValue(25)
    );

    double first =
        system.evaluate(
            inputs,
            "fan"
        );

    double second =
        system.evaluate(
            inputs,
            "fan"
        );


    EXPECT_DOUBLE_EQ(
        first,
        second
    );
}
// ----------------------------------------------------
// Mathematical / Logical Evaluation Tests
// ----------------------------------------------------

TEST_F(
    MamdaniSystemTest,
    ColdInputProducesLowerOutputThanHotInput
)
{
    system.addInputVar(
        createTemperature()
    );

    system.addOutputVar(
        createFan()
    );

    Rule coldRule =
        createColdRule();


    Antecedent hotAnt(
        {
            Condition(
                "temperature",
                "hot"
            )
        }
    );

    Consequent hotCons(
        {
            Condition(
                "fan",
                "fast"
            )
        }
    );

    Rule hotRule(
        hotAnt,
        hotCons
    );


    system.addRule(
        coldRule
    );

    system.addRule(
        hotRule
    );


    std::unordered_map<
        std::string,
        InputValue
    > coldInput;

    coldInput.emplace(
        "temperature",
        InputValue(25)
    );


    std::unordered_map<
        std::string,
        InputValue
    > hotInput;

    hotInput.emplace(
        "temperature",
        InputValue(75)
    );


    double coldResult =
        system.evaluate(
            coldInput,
            "fan"
        );

    double hotResult =
        system.evaluate(
            hotInput,
            "fan"
        );


    EXPECT_LT(
        coldResult,
        hotResult
    );
}



TEST_F(
    MamdaniSystemTest,
    MultipleRulesProduceMiddleOutput
)
{
    system.addInputVar(
        createTemperature()
    );

    system.addOutputVar(
        createFan()
    );


    system.addRule(
        createColdRule()
    );


    system.addRule(
        Rule(
            Antecedent(
                {
                    Condition(
                        "temperature",
                        "hot"
                    )
                }
            ),
            Consequent(
                {
                    Condition(
                        "fan",
                        "fast"
                    )
                }
            )
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
            inputs,
            "fan"
        );


    EXPECT_GT(
        result,
        25
    );

    EXPECT_LT(
        result,
        75
    );
}



// ----------------------------------------------------
// Multiple Variables AND test
// ----------------------------------------------------

TEST_F(
    MamdaniSystemTest,
    MultipleVariablesWithANDAntecedentWorks
)
{
    system.addInputVar(
        createTemperature()
    );


    InputVar humidity(
        "humidity",
        0,
        100
    );

    humidity.addSet(
        FuzzySet(
            "high",
            std::make_shared<Triangle>(
                "high",
                50,
                100,
                100
                )
        )
    );


    system.addInputVar(
        humidity
    );

    system.addOutputVar(
        createFan()
    );


    Antecedent ant(
        {
            Condition(
                "temperature",
                "hot"
            ),

            Condition(
                "humidity",
                "high"
            )
        },
        LogicalOperator::AND
    );


    Consequent cons(
        {
            Condition(
                "fan",
                "fast"
            )
        }
    );


    system.addRule(
        Rule(
            ant,
            cons
        )
    );


    std::unordered_map<
        std::string,
        InputValue
    > inputs;

    inputs.emplace(
        "temperature",
        InputValue(80)
    );

    inputs.emplace(
        "humidity",
        InputValue(80)
    );


    double result =
        system.evaluate(
            inputs,
            "fan"
        );


    EXPECT_GT(
        result,
        50
    );
}



// ----------------------------------------------------
// OR Antecedent Test
// ----------------------------------------------------

TEST_F(
    MamdaniSystemTest,
    ORAntecedentUsesSNorm
)
{
    system.addInputVar(
        createTemperature()
    );


    InputVar humidity(
        "humidity",
        0,
        100
    );

    humidity.addSet(
        FuzzySet(
            "high",
            std::make_shared<Triangle>(
                "high",
                50,
                100,
                100
                )
        )
    );

    system.addInputVar(
        humidity
    );

    system.addOutputVar(
        createFan()
    );


    Antecedent ant(
        {
            Condition(
                "temperature",
                "hot"
            ),

            Condition(
                "humidity",
                "high"
            )
        },
        LogicalOperator::OR
    );


    Consequent cons(
        {
            Condition(
                "fan",
                "fast"
            )
        }
    );


    system.addRule(
        Rule(
            ant,
            cons
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

    inputs.emplace(
        "humidity",
        InputValue(90)
    );


    double result =
        system.evaluate(
            inputs,
            "fan"
        );


    EXPECT_GT(
        result,
        50
    );
}



// ----------------------------------------------------
// Rule Ordering
// ----------------------------------------------------

TEST_F(
    MamdaniSystemTest,
    RuleOrderDoesNotAffectOutput
)
{
    auto engine2 =
        std::make_shared<InferenceEngine>(
            std::make_shared<MinTNorm>(),
            std::make_shared<MaxSNorm>(),
            std::make_shared<CentroidDefuzz>()
            );


    MamdaniSystem first(
        engine
    );

    MamdaniSystem second(
        engine2
    );


    first.addInputVar(
        createTemperature()
    );

    second.addInputVar(
        createTemperature()
    );


    first.addOutputVar(
        createFan()
    );

    second.addOutputVar(
        createFan()
    );


    Rule cold =
        createColdRule();


    Rule hot(
        Antecedent(
            {
                Condition(
                    "temperature",
                    "hot"
                )
            }
        ),

        Consequent(
            {
                Condition(
                    "fan",
                    "fast"
                )
            }
        )
    );


    first.addRule(
        cold
    );

    first.addRule(
        hot
    );


    second.addRule(
        hot
    );

    second.addRule(
        cold
    );


    std::unordered_map<
        std::string,
        InputValue
    > inputs;

    inputs.emplace(
        "temperature",
        InputValue(50)
    );


    EXPECT_NEAR(
        first.evaluate(
            inputs,
            "fan"
        ),

        second.evaluate(
            inputs,
            "fan"
        ),

        1e-6
    );
}



// ----------------------------------------------------
// Different TNorm implementations
// ----------------------------------------------------

TEST_F(
    MamdaniSystemTest,
    ProductAndMinTNormProduceDifferentResults
)
{
    auto productEngine =
        std::make_shared<InferenceEngine>(
            std::make_shared<ProdTNorm>(),
            std::make_shared<MaxSNorm>(),
            std::make_shared<CentroidDefuzz>()
            );


    MamdaniSystem productSystem(
        productEngine
    );


    productSystem.addInputVar(
        createTemperature()
    );


    productSystem.addOutputVar(
        createFan()
    );


    productSystem.addRule(
        createColdRule()
    );


    system.addInputVar(
        createTemperature()
    );

    system.addOutputVar(
        createFan()
    );

    system.addRule(
        createColdRule()
    );


    std::unordered_map<
        std::string,
        InputValue
    > inputs;

    inputs.emplace(
        "temperature",
        InputValue(25)
    );


    double minResult =
        system.evaluate(
            inputs,
            "fan"
        );


    double productResult =
        productSystem.evaluate(
            inputs,
            "fan"
        );


    EXPECT_NEAR(
        minResult,
        productResult,
        20
    );
}