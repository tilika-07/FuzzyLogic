#include "../include/FuzzyLogic.h"
#include <iostream>
#include <unordered_map>

int main()
{
    MamdaniSystem system;
    //input var1=CPU temp
    InputVar temperature("Temperature", 30.0, 100.0);

    temperature.addSet(
        FuzzySet(
            "Cold",
            std::make_shared<Triangle>("cold",0.0, 30.0, 45.0)
        )
    );

    temperature.addSet(
        FuzzySet(
            "Warm",
            std::make_shared<Triangle>("warm",40.0, 60.0, 80.0)
        )
    );

    temperature.addSet(
        FuzzySet(
            "Hot",
            std::make_shared<Triangle>("Hot",70.0, 100.0, 100.0)
        )
    );

    system.addInputVar(temperature);

   //input var2=cpu load or power dissip

    InputVar load("Load", 0.0, 100.0);

    load.addSet(
        FuzzySet(
            "Low",
            std::make_shared<Triangle>("Low",0.0, 0.0, 40.0)
        )
    );

    load.addSet(
        FuzzySet(
            "Medium",
            std::make_shared<Triangle>("Medium",20.0, 50.0, 80.0)
        )
    );

    load.addSet(
        FuzzySet(
            "High",
            std::make_shared<Triangle>("High",60.0, 100.0, 100.0)
        )
    );

    system.addInputVar(load);

    //output var=fan speed

    OutputVar fan("FanSpeed", 0.0, 100.0);

    fan.addSet(
        FuzzySet(
            "Low",
            std::make_shared<Triangle>("Low",0.0, 0.0, 35.0)
        )
    );

    fan.addSet(
        FuzzySet(
            "Medium",
            std::make_shared<Triangle>("Medium",25.0, 50.0, 75.0)
        )
    );

    fan.addSet(
        FuzzySet(
            "High",
            std::make_shared<Triangle>("High",60.0, 80.0, 95.0)
        )
    );

    fan.addSet(
        FuzzySet(
            "Maximum",
            std::make_shared<Triangle>("Maximum",85.0, 100.0, 100.0)
        )
    );

    system.addOutputVar(fan);
    //adding rules

    system.addRule(
        Rule(
            Antecedent(
                {
                    Condition("Temperature", "Cold"),
                    Condition("Load", "Low")
                },
                LogicalOperator::AND
            ),
            Consequent(
                {
                    Condition("FanSpeed", "Low")
                }
            )
        )
    );


    system.addRule(
        Rule(
            Antecedent(
                {
                    Condition("Temperature", "Warm"),
                    Condition("Load", "Low")
                },
                LogicalOperator::AND
            ),
            Consequent(
                {
                    Condition("FanSpeed", "Medium")
                }
            )
        )
    );

    system.addRule(
        Rule(
            Antecedent(
                {
                    Condition("Temperature", "Warm"),
                    Condition("Load", "Medium")
                },
                LogicalOperator::AND
            ),
            Consequent(
                {
                    Condition("FanSpeed", "High")
                }
            )
        )
    );



    system.addRule(
        Rule(
            Antecedent(
                {
                    Condition("Temperature", "Hot")
                },
                LogicalOperator::AND
            ),
            Consequent(
                {
                    Condition("FanSpeed", "Maximum")
                }
            )
        )
    );

    system.addRule(
        Rule(
            Antecedent(
                {
                    Condition("Load", "High")
                },
                LogicalOperator::AND
            ),
            Consequent(
                {
                    Condition("FanSpeed", "High")
                }
            )
        )
    );

    system.addRule(
        Rule(
            Antecedent(
                {
                    Condition("Temperature", "Hot"),
                    Condition("Load", "High")
                },
                LogicalOperator::AND
            ),
            Consequent(
                {
                    Condition("FanSpeed", "Maximum")
                }
            )
        )
    );

//creating test cases

    std::vector<
        std::pair<
        std::string,
        std::unordered_map<std::string, InputValue>
        >
    > tests =
    {
        {
            "Idle Server",
            {
                {"Temperature", InputValue(35.0)},
                {"Load", InputValue(15.0)}
            }
        },

        {
            "Office Workload",
            {
                {"Temperature", InputValue(55.0)},
                {"Load", InputValue(45.0)}
            }
        },

        {
            "Compiling Large Project",
            {
                {"Temperature", InputValue(72.0)},
                {"Load", InputValue(78.0)}
            }
        },

        {
            "AI Model Training",
            {
                {"Temperature", InputValue(92.0)},
                {"Load", InputValue(98.0)}
            }
        }
    };
//running test cases

    for (const auto& test : tests)
    {
        double result =
            system.evaluate(
                test.second,
                "FanSpeed"
            );

        std::cout << test.first << '\n';
        std::cout << "Temperature : "
            << test.second.at("Temperature").getCrispValue()
            << " C\n";

        std::cout << "CPU Load    : "
            << test.second.at("Load").getCrispValue()
            << " %\n";

        std::cout << "Fan Speed   : "
            << result
            << " %\n";
    }

    return 0;
}