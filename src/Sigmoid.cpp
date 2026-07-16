#include "../include/Sigmoid.h"

#include <cmath>

Sigmoid::Sigmoid(
    const std::string& name,
    double a,
    double c)
    : name_(name),
      a_(a),
      c_(c)
{
}

double Sigmoid::compute(double x) const
{
    return 1.0 /
           (1.0 + std::exp(-a_ * (x - c_)));
}

std::string Sigmoid::getName() const
{
    return name_;
}