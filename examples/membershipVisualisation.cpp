#include <iostream>
#include "FuzzyLogic.h"
#include "visualization/JsonExporter.h"

using namespace fuzzy;

int main()
{
    FuzzyVariable temperature("Temperature", 0.0, 100.0);
    //diff membership functions taken only for example
    temperature.addSet(
        FuzzySet(
            "Cold",
            std::make_shared<Triangle>("Cold",0.0, 40.0, 60.0)
        )
    );

    temperature.addSet(
        FuzzySet(
            "Warm",
            std::make_shared<Gaussian>("Warm",50.0, 12.0)
        )
    );

    temperature.addSet(
        FuzzySet(
            "Hot",
            std::make_shared<Trapezoidal>(
                "Hot",
                60.0,
                75.0,
                100.0,
                100.0
                )
        )
    );

    constexpr double samplingStep = 0.5;

    fuzzy::visualization::JsonExporter::exportMembershipFunctions(
        temperature,
        "examples/output/membership.json",
        samplingStep
    );

    std::cout
        << "Membership visualization exported to:\n"
        << "examples/output/membership.json\n";

    return 0;
}