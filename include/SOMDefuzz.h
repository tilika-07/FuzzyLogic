#pragma once

#include "Defuzzifier.h"

class SOMDefuzz: public Defuzzifier
{
public:
    double defuzzify(
        const std::vector<double>& xValues,
        const std::vector<double>& memberships
    ) const override;
};