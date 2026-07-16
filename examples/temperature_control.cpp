#include <iostream>
#include <string>
#include "../include/FuzzySystem.h"
#include "../include/Triangle.h"

int main()
{
    FuzzyVariable temperature(
        "Temperature");

    temperature.addSet(
        FuzzySet(
            "Cold",
            std::make_unique<
                Triangle>(
                    "Cold",
                    0,
                    0,
                    50)));

    temperature.addSet(
        FuzzySet(
            "Warm",
            std::make_unique<
                Triangle>(
                    "Warm",
                    25,
                    50,
                    75)));

    temperature.addSet(
        FuzzySet(
            "Hot",
            std::make_unique<
                Triangle>(
                    "Hot",
                    50,
                    100,
                    100)));

    auto result =
        FuzzySystem::fuzzify(
            temperature,
            60);

    for (auto& [name,val] : result)
    {
        std::cout
            << name
            << " : "
            << val
            << '\n';
    }
}