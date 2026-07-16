#include <gtest/gtest.h>
#include "../include/FuzzyLogic.h"
#include <memory>
#include <unordered_map>
//mamdani controller
TEST(IntegrationTest,MamdaniFanControllerEndToEnd)
{
    auto engine =
        std::make_shared<InferenceEngine>(
            std::make_shared<MinTNorm>(),
            std::make_shared<MaxSNorm>(),
            std::make_shared<CentroidDefuzz>()
            );
    MamdaniSystem system(engine);
    InputVar temperature(
        "temperature",
        0,      //min value
        100    //max value
    );
    temperature.addSet(
        FuzzySet(
            "cold",
            std::make_shared<Triangle>("cold",0,0,70)
        )
    );


    temperature.addSet(
        FuzzySet(
            "hot",
            std::make_shared<Triangle>("hot",30,100,100)
        )
    );
    OutputVar fan("fan", 0, 100);

    fan.addSet(
        FuzzySet("slow", std::make_shared<Triangle>("slow",0,0,70)
        )
    );


    fan.addSet(
        FuzzySet(
            "fast",
            std::make_shared<Triangle>("fast",30,100,100)
        )
    );
    system.addInputVar(
        temperature
    );
    system.addOutputVar(
        fan
    );
    system.addRule(
        Rule(
            Antecedent(
                {
                    Condition(
                        "temperature",
                        "cold"
                    )
                }
            ),
            Consequent(
                {
                    Condition(
                        "fan",
                        "slow"
                    )
                }
            )
        )
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
    std::unordered_map<std::string,InputValue>coldInput;


    coldInput.emplace(
        "temperature",
        InputValue(20)
    );
    std::unordered_map<std::string,InputValue>hotInput;
    hotInput.emplace(
        "temperature",
        InputValue(90)
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
//sugeno controller
TEST(IntegrationTest,SugenoControllerEndToEnd){
    auto engine =
        std::make_shared<SugenoInference>(
            RuleEvaluator(),
            std::make_shared<MinTNorm>(),
            std::make_shared<MaxSNorm>()
            );
    SugenoSystem system(engine);
    InputVar temperature(
        "temperature", 0, 100);
    temperature.addSet(
        FuzzySet("cold",
            std::make_shared<Triangle>("cold",0,0,70))
    );
    temperature.addSet(
        FuzzySet(
            "hot",
            std::make_shared<Triangle>("hot",30,100,100)
        )
    );


    system.addInputVar(
        temperature
    );


    system.addRule(
        SugenoRule(
            Antecedent(
                {
                    Condition(
                        "temperature",
                        "cold"
                    )
                }
            ),
            std::make_shared<ConstSugenoConsequent>(
                10
                )
        )
    );


    system.addRule(
        SugenoRule(
            Antecedent(
                {
                    Condition(
                        "temperature",
                        "hot"
                    )
                }
            ),
            std::make_shared<ConstSugenoConsequent>(
                50
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
            inputs
        );
    EXPECT_NEAR(
        result,
        30.0,
        1e-3
    );
}
TEST(
    IntegrationTest,
    SugenoLinearConsequentEndToEnd)
{
    auto engine =
        std::make_shared<SugenoInference>(
            RuleEvaluator(),
            std::make_shared<MinTNorm>(),
            std::make_shared<MaxSNorm>()
            );


    SugenoSystem system(
        engine
    );


    InputVar temp(
        "temperature",
        0,
        100
    );


    temp.addSet(
        FuzzySet(
            "normal",
            std::make_shared<Triangle>(
                "normal",
                0,
                50,
                100
                )
        )
    );


    system.addInputVar(
        temp
    );


    std::unordered_map<
        std::string,
        double
    > coeffs;


    coeffs.emplace(
        "temperature",
        2
    );


    system.addRule(
        SugenoRule(
            Antecedent(
                {
                    Condition(
                        "temperature",
                        "normal"
                    )
                }
            ),
            std::make_shared<LinearSugenoConsequent>(
                coeffs,
                5
                )
        )
    );


    std::unordered_map<
        std::string,
        InputValue
    > inputs;


    inputs.emplace(
        "temperature",
        InputValue(40)
    );


    EXPECT_NEAR(
        system.evaluate(inputs),
        85,
        1e-6
    );
}