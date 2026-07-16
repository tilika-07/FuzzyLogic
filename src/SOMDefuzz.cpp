#include "../include/SOMDefuzz.h"

#include <algorithm>
#include <stdexcept>

double SOMDefuzz::defuzzify(
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

    for (size_t i = 0; i < memberships.size(); ++i)
    {
        if (memberships[i] == maxMembership)
        {
            return xValues[i];
        }
    }

    return 0.0;
}