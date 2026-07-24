#include "../include/FuzzyLogic.h"
#include <iostream>
#include <unordered_map>
int main()
{
    SugenoSystem system;

    //input var1=CPU temp

    InputVar temperature("Temperature", 30.0, 100.0);

    temperature.addSet(
        FuzzySet(
            "Low",
            std::make_shared<Triangle>("Low",30.0, 30.0, 55.0)
        )
    );

    temperature.addSet(
        FuzzySet(
            "Medium",
            std::make_shared<Triangle>("edium",45.0, 65.0, 85.0)
        )
    );

    temperature.addSet(
        FuzzySet(
            "High",
            std::make_shared<Triangle>("High",75.0, 100.0, 100.0)
        )
    );

    system.addInputVar(temperature);

//input var2=CPU Load
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

 //adding rules
    system.addRule(
        SugenoRule(
            Antecedent(
                {
                    Condition("Temperature", "Low"),
                    Condition("Load", "Low")
                },
                LogicalOperator::AND
            ),
            std::make_shared<ConstSugenoConsequent>(2.2)        //ouptut var=CPU freq
        )
    );
    system.addRule(
        SugenoRule(
            Antecedent(
                {
                    Condition("Temperature", "Low"),
                    Condition("Load", "Medium")
                },
                LogicalOperator::AND
            ),
            std::make_shared<ConstSugenoConsequent>(2.8)
        )
    );
    system.addRule(
        SugenoRule(
            Antecedent(
                {
                    Condition("Temperature", "Medium"),
                    Condition("Load", "Medium")
                },
                LogicalOperator::AND
            ),
            std::make_shared<ConstSugenoConsequent>(3.3)        //for constant as output
        )
    );
    system.addRule(
        SugenoRule(
            Antecedent(     //optionally create Antecedent var sep then pass, for cleaner code
                {
                    Condition("Temperature", "High")
                },
                LogicalOperator::AND
            ),
            std::make_shared<LinearSugenoConsequent>(
                std::unordered_map<std::string, double>{
        {"Temperature", -0.010},            //varName& coeffs
        { "Load", 0.015 }
    }, 3.8)   //bias
 ) );

    system.addRule(
        SugenoRule(
            Antecedent(
                {
                    Condition("Load", "High")
                },
                LogicalOperator::AND
            ),
            std::make_shared<LinearSugenoConsequent>(
                std::unordered_map<std::string, double>
    {
        {"Temperature", -0.012},
        { "Load", 0.018 }
    },
                3.6
            )
        )
    );
    system.addRule(
        SugenoRule(
            Antecedent(
                {
                    Condition("Temperature", "High"),
                    Condition("Load", "High")
                },
                LogicalOperator::AND
            ),
            std::make_shared<ConstSugenoConsequent>(2.4)
        )
    );
//test

    std::vector<
        std::pair<
        std::string,
        std::unordered_map<std::string, InputValue>
        >
    > tests =
    {
        {
            "Office Workload",
            {
                {"Temperature", InputValue(40.0)},
                {"Load", InputValue(25.0)}
            }
        },

        {
            "Software Compilation",
            {
                {"Temperature", InputValue(60.0)},
                {"Load", InputValue(55.0)}
            }
        },

        {
            "Gaming",
            {
                {"Temperature", InputValue(76.0)},
                {"Load", InputValue(82.0)}
            }
        },

        {
            "AI Model Training",
            {
                {"Temperature", InputValue(94.0)},
                {"Load", InputValue(99.0)}
            }
        }
    };

 
    std::cout << " CPU Frequency Controller (Sugeno FIS)\n";
   
    for (const auto& test : tests)
    {
        double frequency = system.evaluate(test.second);

        std::cout << test.first << '\n';
        //displaying input vars
        std::cout << "Temperature : "           
            << test.second.at("Temperature").getCrispValue()
            << " C\n";

        std::cout << "CPU Load    : "
            << test.second.at("Load").getCrispValue()
            << " %\n";
        //displaying output vars
        std::cout << "CPU Frequency : "
            << frequency
            << " GHz\n";

        
    }

    return 0;
}