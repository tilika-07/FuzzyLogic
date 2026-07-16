#ifndef PROBABILISTIC_SUM_SNORM_H
#define PROBABILISTIC_SUM_SNORM_H

#include "SNorm.h"

class ProbSumSNorm : public SNorm
{
public:
    double apply(double a, double b) const override;
};

#endif