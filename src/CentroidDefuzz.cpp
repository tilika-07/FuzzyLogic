#include "../include/CentroidDefuzz.h"
#include <stdexcept>

double CentroidDefuzz::defuzzify(
    const std::vector<double>& xValues,
    const std::vector<double>& memberships
) const
{
    if (xValues.empty() || memberships.empty())
    {
        throw std::invalid_argument("Empty aggregated set");
    }

    if (xValues.size() != memberships.size())
    {
        throw std::invalid_argument("Size mismatch");
    }

    double numerator = 0.0;
    double denominator = 0.0;

    for (size_t i = 0; i < xValues.size(); ++i)
    {
        numerator += xValues[i] * memberships[i];
        denominator += memberships[i];
    }

    if (denominator == 0.0)
    {
        throw std::runtime_error("Zero total membership");
    }

    return numerator / denominator;
}