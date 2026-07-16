#include "../include/BisectorDefuzz.h"

#include <stdexcept>

double BisectorDefuzz::defuzzify(
    const std::vector<double>& xValues,
    const std::vector<double>& memberships
) const
{
    if (xValues.size() != memberships.size())
    {
        throw std::runtime_error("Size mismatch");
    }

    double totalArea = 0.0;

    for (double mu : memberships)
    {
        totalArea += mu;
    }

    double halfArea = totalArea / 2.0;

    double accumulated = 0.0;

    for (size_t i = 0; i < memberships.size(); ++i)
    {
        accumulated += memberships[i];

        if (accumulated >= halfArea)
        {
            return xValues[i];
        }
    }

    return xValues.back();
}