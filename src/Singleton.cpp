#include "../include/Singleton.h"

#include <cmath>

Singleton::Singleton(
    const std::string& name,
    double center)
    : name_(name),
      center_(center)
{
}

double Singleton::compute(double x) const
{
    constexpr double EPS = 1e-9;

    return (std::abs(x - center_) < EPS)
               ? 1.0
               : 0.0;
}

std::string Singleton::getName() const
{
    return name_;
}