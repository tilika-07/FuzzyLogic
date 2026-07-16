#include "../include/MOMDefuzz.h"

#include <algorithm>
#include <stdexcept>

double MOMDefuzz::defuzzify(
    const std::vector<double>& xValues,
    const std::vector<double>& memberships
) const
{
    if (xValues.size() != memberships.size())
    {
        throw std::runtime_error("Size mismatch");
    }

    double maxMembership =
        *std::max_element(
            memberships.begin(),
            memberships.end()
        );

    double sum = 0.0;
    int count = 0;

    for (size_t i = 0; i < memberships.size(); ++i)
    {
        if (memberships[i] == maxMembership)
        {
            sum += xValues[i];
            ++count;
        }
    }

    if (count == 0)
    {
        return 0.0;
    }

    return sum / count;
}