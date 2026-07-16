#include "../include/Bell.h"

#include <cmath>

Bell::Bell(
    const std::string& name,
    double a,
    double b,
    double c)
    : name_(name),
      a_(a),
      b_(b),
      c_(c)
{
}

double Bell::compute(double x) const
{
    double value =
        std::abs((x - c_) / a_);

    return 1.0 /
           (1.0 + std::pow(value, 2.0 * b_));
}

std::string Bell::getName() const
{
    return name_;
}