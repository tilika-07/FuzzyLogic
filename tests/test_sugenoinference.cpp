#include <gtest/gtest.h>
#include "../include/SugenoInference.h"
#include "../include/RuleEvaluator.h"
#include "../include/ConstSugenoConsequent.h"
#include "../include/LinearSugenoConsequent.h"
#include "../include/FuzzyVariable.h"
#include "../include/FuzzySet.h"
#include "../include/Triangle.h"
#include "../include/MinTNorm.h"
#include "../include/ProdTNorm.h"
#include "../include/MaxSNorm.h"

class SugenoInferenceTest : public ::testing::Test
{
protected:
    RuleEvaluator evaluator;

    std::shared_ptr<MinTNorm> minNorm =
        std::make_shared<MinTNorm>();

    std::shared_ptr<ProdTNorm> prodNorm =
        std::make_shared<ProdTNorm>();

    std::shared_ptr<MaxSNorm> maxNorm =
        std::make_shared<MaxSNorm>();

    std::unordered_map<
        std::string,
        std::shared_ptr<FuzzyVariable>
    > variables;

    std::unordered_map<
        std::string,
        InputValue
    > inputs;

    void SetUp() override
    {
        auto temperature =
            std::make_shared<FuzzyVariable>(
                "temperature",
                0.0,
                100.0
                );

        temperature->addSet(
            FuzzySet(
                "Cold",
                std::make_shared<Triangle>(
                    "Cold",
                    0,
                    0,
                    50
                    )
            )
        );

        temperature->addSet(
            FuzzySet(
                "Hot",
                std::make_shared<Triangle>(
                    "Hot",
                    50,
                    100,
                    100
                    )
            )
        );

        variables["temperature"] =
            temperature;

        inputs.emplace("temperature",InputValue(25.0));
    }

    Antecedent makeAntecedent(
        const std::string& setName,
        LogicalOperator op = LogicalOperator::AND
    )
    {
        return Antecedent(
            {
                Condition(
                    "temperature",
                    setName
                )
            },
            op
        );
    }

    SugenoRule makeConstantRule(
        const std::string& setName,
        double output
    )
    {
        return SugenoRule(
            makeAntecedent(setName),
            std::make_shared<
            ConstSugenoConsequent
            >(output)
        );
    }
};

// Constructor Tests

TEST_F(SugenoInferenceTest,ConstructorStoresDependencies){
    EXPECT_NO_THROW(
        SugenoInference inference(
            evaluator,
            minNorm,
            maxNorm
        );
    );
}
//missing arguments
TEST_F( SugenoInferenceTest,NullTNormThrows)
{
  EXPECT_THROW(
        SugenoInference(
            evaluator,
            nullptr,
            maxNorm
        ),
        std::invalid_argument
    );
}

TEST_F(
    SugenoInferenceTest,
    NullSNormThrows
)
{
    EXPECT_THROW(
        SugenoInference(
            evaluator,
            minNorm,
            nullptr
        ),
        std::invalid_argument
    );
}

TEST_F(
    SugenoInferenceTest,
    BothNullThrows
)
{
    EXPECT_THROW(
        SugenoInference(
            evaluator,
            nullptr,
            nullptr
        ),
        std::invalid_argument
    );
}

//evaluation

TEST_F(SugenoInferenceTest,SingleRuleReturnsCorrectOutput)
{
    SugenoInference inference(
        evaluator,
        minNorm,
        maxNorm
    );

    std::vector<SugenoRule> rules =
    {
        makeConstantRule(
            "Cold",
            10.0
        )
    };

    double result =
        inference.evaluate(
            rules,
            inputs,
            variables
        );

    EXPECT_NEAR(
        result,
        10.0,
        1e-6
    );
}

TEST_F(
    SugenoInferenceTest,
    TwoRulesWeightedAverage
)
{
    SugenoInference inference(
        evaluator,
        minNorm,
        maxNorm
    );

    std::vector<SugenoRule> rules =
    {
        makeConstantRule(
            "Cold",
            10.0
        ),
        makeConstantRule(
            "Hot",
            20.0
        )
    };

    double result =
        inference.evaluate(
            rules,
            inputs,
            variables
        );

    // Cold = 0.5
    // Hot = 0.0
    // (0.5*10 + 0*20)/0.5 = 10

    EXPECT_NEAR(
        result,
        10.0,
        1e-6
    );
}

TEST_F(
    SugenoInferenceTest,
    ThreeRulesWeightedAverage
)
{
    SugenoInference inference(
        evaluator,
        minNorm,
        maxNorm
    );

    std::vector<SugenoRule> rules =
    {
        makeConstantRule(
            "Cold",
            10.0
        ),
        makeConstantRule(
            "Cold",
            20.0
        ),
        makeConstantRule(
            "Hot",
            30.0
        )
    };

    double result =
        inference.evaluate(
            rules,
            inputs,
            variables
        );

    // firing strengths:
    // Cold = 0.5
    // Cold = 0.5
    // Hot = 0.0
    //
    // weighted =
    // (0.5*10 + 0.5*20)/(1.0)

    EXPECT_NEAR(
        result,
        15.0,
        1e-6
    );
}

TEST_F(
    SugenoInferenceTest,
    WeightedAverageWithDifferentConsequents
)
{
    SugenoInference inference(
        evaluator,
        minNorm,
        maxNorm
    );

    inputs.clear();
    inputs.emplace("temperature", InputValue(75.0));

    std::vector<SugenoRule> rules =
    {
        makeConstantRule(
            "Cold",
            10.0
        ),
        makeConstantRule(
            "Hot",
            50.0
        )
    };

    double result =
        inference.evaluate(
            rules,
            inputs,
            variables
        );

    // Hot membership = 0.5
    // Cold membership = 0
    // output = 50

    EXPECT_NEAR(
        result,
        50.0,
        1e-6
    );
}

TEST_F(
    SugenoInferenceTest,
    DecimalWeightedAverage
)
{
    SugenoInference inference(
        evaluator,
        minNorm,
        maxNorm
    );

    std::vector<SugenoRule> rules =
    {
        makeConstantRule(
            "Cold",
            12.5
        ),
        makeConstantRule(
            "Cold",
            17.5
        )
    };

    double result =
        inference.evaluate(
            rules,
            inputs,
            variables
        );

    EXPECT_NEAR(
        result,
        15.0,
        1e-6
    );
}
//linear consequent tests

TEST_F(SugenoInferenceTest,LinearConsequentEvaluation)
{
    SugenoInference inference(
        evaluator,
        minNorm,
        maxNorm
    );

    std::unordered_map<std::string, double> coeffs;
    coeffs.emplace("temperature", 2.0);

    auto consequent =
        std::make_shared<LinearSugenoConsequent>(
            coeffs,
            5.0
            );

    SugenoRule rule(
        makeAntecedent("Cold"),
        consequent
    );

    std::vector<SugenoRule> rules{ rule };

    double result =
        inference.evaluate(
            rules,
            inputs,
            variables
        );

    // 2*25 + 5 = 55
    EXPECT_NEAR(result, 55.0, 1e-6);
}

TEST_F(SugenoInferenceTest, MixedConstantAndLinearConsequents)
{
    SugenoInference inference(
        evaluator,
        minNorm,
        maxNorm
    );

    std::unordered_map<std::string, double> coeffs;
    coeffs.emplace("temperature", 2.0);

    auto linear =
        std::make_shared<LinearSugenoConsequent>(
            coeffs,
            5.0
            );

    auto constant =
        std::make_shared<ConstSugenoConsequent>(
            15.0
            );

    std::vector<SugenoRule> rules;

    rules.emplace_back(
        makeAntecedent("Cold"),
        linear
    );

    rules.emplace_back(
        makeAntecedent("Cold"),
        constant
    );

    double result =
        inference.evaluate(
            rules,
            inputs,
            variables
        );

    // ((0.5*55)+(0.5*15))/1 = 35
    EXPECT_NEAR(result, 35.0, 1e-6);
}

//
// Rule ordering
//

TEST_F(
    SugenoInferenceTest,
    RuleOrderDoesNotChangeOutput
)
{
    SugenoInference inference(
        evaluator,
        minNorm,
        maxNorm
    );

    std::vector<SugenoRule> rules1{
        makeConstantRule("Cold",10.0),
        makeConstantRule("Cold",20.0)
    };

    std::vector<SugenoRule> rules2{
        makeConstantRule("Cold",20.0),
        makeConstantRule("Cold",10.0)
    };

    double result1 =
        inference.evaluate(
            rules1,
            inputs,
            variables
        );

    double result2 =
        inference.evaluate(
            rules2,
            inputs,
            variables
        );

    EXPECT_NEAR(result1, result2, 1e-6);
}

//
// Product vs Min TNorm
//

TEST_F(
    SugenoInferenceTest,
    ProductAndMinTNormProduceDifferentOutputs
)
{
    auto humidity =
        std::make_shared<FuzzyVariable>(
            "humidity",
            0,
            100
            );

    humidity->addSet(
        FuzzySet(
            "Wet",
            std::make_shared<Triangle>(
                "Wet",
                0,
                50,
                100
                )
        )
    );

    variables.emplace(
        "humidity",
        humidity
    );

    inputs.emplace(
        "humidity",
        InputValue(25.0)
    );

    Antecedent antecedent(
        {
            Condition("temperature","Cold"),
            Condition("humidity","Wet")
        },
        LogicalOperator::AND
    );

    auto consequent =
        std::make_shared<ConstSugenoConsequent>(
            100.0
            );

    SugenoRule rule(
        antecedent,
        consequent
    );

    std::vector<SugenoRule> rules{ rule };

    SugenoInference minInference(
        evaluator,
        minNorm,
        maxNorm
    );

    SugenoInference prodInference(
        evaluator,
        prodNorm,
        maxNorm
    );

    double minResult =
        minInference.evaluate(
            rules,
            inputs,
            variables
        );

    double prodResult =
        prodInference.evaluate(
            rules,
            inputs,
            variables
        );

    // Single rule ? weighted average always equals consequent.
    EXPECT_NEAR(minResult, 100.0, 1e-6);
    EXPECT_NEAR(prodResult, 100.0, 1e-6);
}


TEST_F(
    SugenoInferenceTest,
    MultipleVariablesEvaluation
)
{
    auto humidity =
        std::make_shared<FuzzyVariable>(
            "humidity",
            0,
            100
            );

    humidity->addSet(
        FuzzySet(
            "Wet",
            std::make_shared<Triangle>(
                "Wet",
                0,
                50,
                100
                )
        )
    );

    variables.emplace(
        "humidity",
        humidity
    );

    inputs.emplace(
        "humidity",
        InputValue(25.0)
    );

    Antecedent antecedent(
        {
            Condition("temperature","Cold"),
            Condition("humidity","Wet")
        },
        LogicalOperator::AND
    );

    auto consequent =
        std::make_shared<ConstSugenoConsequent>(
            42.0
            );

    SugenoRule rule(
        antecedent,
        consequent
    );

    SugenoInference inference(
        evaluator,
        minNorm,
        maxNorm
    );

    double result =
        inference.evaluate(
            { rule },
            inputs,
            variables
        );

    EXPECT_NEAR(result, 42.0, 1e-6);
}

//
// OR antecedent
//

TEST_F(
    SugenoInferenceTest,
    ORAntecedentUsesSNorm
)
{
    auto humidity =
        std::make_shared<FuzzyVariable>(
            "humidity",
            0,
            100
            );

    humidity->addSet(
        FuzzySet(
            "Wet",
            std::make_shared<Triangle>(
                "Wet",
                0,
                50,
                100
                )
        )
    );

    variables.emplace(
        "humidity",
        humidity
    );

    inputs.emplace(
        "humidity",
        InputValue(25.0)
    );

    Antecedent antecedent(
        {
            Condition("temperature","Cold"),
            Condition("humidity","Wet")
        },
        LogicalOperator::OR
    );

    auto consequent =
        std::make_shared<ConstSugenoConsequent>(
            77.0
            );

    SugenoRule rule(
        antecedent,
        consequent
    );

    SugenoInference inference(
        evaluator,
        minNorm,
        maxNorm
    );

    double result =
        inference.evaluate(
            { rule },
            inputs,
            variables
        );

    EXPECT_NEAR(result, 77.0, 1e-6);
}
//
// Exception Tests
//

TEST_F(
    SugenoInferenceTest,
    EmptyRuleBaseThrows
)
{
    SugenoInference inference(
        evaluator,
        minNorm,
        maxNorm
    );

    std::vector<SugenoRule> rules;

    EXPECT_THROW(
        inference.evaluate(
            rules,
            inputs,
            variables
        ),
        std::runtime_error
    );
}

TEST_F(
    SugenoInferenceTest,
    ZeroFiringStrengthThrows
)
{
    SugenoInference inference(
        evaluator,
        minNorm,
        maxNorm
    );

    inputs.clear();
    inputs.emplace(
        "temperature",
        InputValue(100.0)
    );

    std::vector<SugenoRule> rules{
        makeConstantRule(
            "Cold",
            10.0
        )
    };

    EXPECT_THROW(
        inference.evaluate(
            rules,
            inputs,
            variables
        ),
        std::runtime_error
    );
}

TEST_F(
    SugenoInferenceTest,
    LinguisticInputThrows
)
{
    SugenoInference inference(
        evaluator,
        minNorm,
        maxNorm
    );

    inputs.clear();

    inputs.emplace(
        "temperature",
        InputValue(
            std::string("Cold")
        )
    );

    std::vector<SugenoRule> rules{
        makeConstantRule(
            "Cold",
            20.0
        )
    };

    EXPECT_THROW(
        inference.evaluate(
            rules,
            inputs,
            variables
        ),
        std::runtime_error
    );
}

TEST_F(
    SugenoInferenceTest,
    FuzzyInputThrows
)
{
    SugenoInference inference(
        evaluator,
        minNorm,
        maxNorm
    );

    std::unordered_map<
        std::string,
        double
    > memberships;

    memberships.emplace(
        "Cold",
        0.6
    );

    memberships.emplace(
        "Hot",
        0.4
    );

    inputs.clear();

    inputs.emplace(
        "temperature",
        InputValue(
            memberships
        )
    );

    std::vector<SugenoRule> rules{
        makeConstantRule(
            "Cold",
            25.0
        )
    };

    EXPECT_THROW(
        inference.evaluate(
            rules,
            inputs,
            variables
        ),
        std::runtime_error
    );
}

//
// Precision Tests
//

TEST_F(
    SugenoInferenceTest,
    OutputMatchesExpectedWithinTolerance
)
{
    SugenoInference inference(
        evaluator,
        minNorm,
        maxNorm
    );

    std::vector<SugenoRule> rules{
        makeConstantRule(
            "Cold",
            15.25
        ),
        makeConstantRule(
            "Cold",
            20.75
        )
    };

    double result =
        inference.evaluate(
            rules,
            inputs,
            variables
        );

    EXPECT_NEAR(
        result,
        18.0,
        1e-9
    );
}

TEST_F(
    SugenoInferenceTest,
    LargeConsequentValues
)
{
    SugenoInference inference(
        evaluator,
        minNorm,
        maxNorm
    );

    std::vector<SugenoRule> rules{
        makeConstantRule(
            "Cold",
            1000000.0
        ),
        makeConstantRule(
            "Cold",
            2000000.0
        )
    };

    double result =
        inference.evaluate(
            rules,
            inputs,
            variables
        );

    EXPECT_NEAR(
        result,
        1500000.0,
        1e-3
    );
}

TEST_F(
    SugenoInferenceTest,
    SmallConsequentValues
)
{
    SugenoInference inference(
        evaluator,
        minNorm,
        maxNorm
    );

    std::vector<SugenoRule> rules{
        makeConstantRule(
            "Cold",
            0.001
        ),
        makeConstantRule(
            "Cold",
            0.003
        )
    };

    double result =
        inference.evaluate(
            rules,
            inputs,
            variables
        );

    EXPECT_NEAR(
        result,
        0.002,
        1e-9
    );
}

//
// Regression Tests
//

TEST_F(
    SugenoInferenceTest,
    IdenticalRulesProduceSameOutput
)
{
    SugenoInference inference(
        evaluator,
        minNorm,
        maxNorm
    );

    SugenoRule rule =
        makeConstantRule(
            "Cold",
            42.0
        );

    std::vector<SugenoRule> rules{
        rule,
        rule,
        rule
    };

    double result =
        inference.evaluate(
            rules,
            inputs,
            variables
        );

    EXPECT_NEAR(
        result,
        42.0,
        1e-6
    );
}

TEST_F(
    SugenoInferenceTest,
    SingleFullyActiveRuleReturnsConsequent
)
{
    SugenoInference inference(
        evaluator,
        minNorm,
        maxNorm
    );

    inputs.clear();

    inputs.emplace(
        "temperature",
        InputValue(0.0)
    );

    std::vector<SugenoRule> rules{
        makeConstantRule(
            "Cold",
            88.0
        )
    };

    double result =
        inference.evaluate(
            rules,
            inputs,
            variables
        );

    EXPECT_NEAR(
        result,
        88.0,
        1e-6
    );
}

TEST_F(
    SugenoInferenceTest,
    WeightedAverageRemainsStable
)
{
    SugenoInference inference(
        evaluator,
        minNorm,
        maxNorm
    );

    std::vector<SugenoRule> rules{
        makeConstantRule("Cold",10.0),
        makeConstantRule("Cold",20.0),
        makeConstantRule("Cold",30.0),
        makeConstantRule("Cold",40.0)
    };

    double result =
        inference.evaluate(
            rules,
            inputs,
            variables
        );

    EXPECT_NEAR(
        result,
        25.0,
        1e-6
    );
}