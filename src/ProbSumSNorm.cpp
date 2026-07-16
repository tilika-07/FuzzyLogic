#include "../include/ProbSumSNorm.h"

double ProbSumSNorm::apply(double a, double b) const
{
    return a + b - (a * b);
}