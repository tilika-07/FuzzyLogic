#pragma once

#include "MembershipFunc.h"

class Sigmoid : public MembershipFunc
{
private:
    std::string name_;

    double a_;
    double c_;

public:
    Sigmoid(
        const std::string& name,
        double a,
        double c);

    double compute(double x) const override;

    std::string getName() const override;
};