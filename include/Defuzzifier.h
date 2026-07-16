#pragma once
#include <vector>
class Defuzzifier
{
public:
    virtual ~Defuzzifier() = default;

    virtual double defuzzify(
        const std::vector<double>& xValues,
        const std::vector<double>& memberships
    ) const = 0;
};