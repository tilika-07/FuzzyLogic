#pragma once

#include "MembershipFunc.h"

class Bell : public MembershipFunc{
private:
    std::string name_;

    double a_;
    double b_;
    double c_;

public:
    Bell(
        const std::string& name,
        double a,
        double b,
        double c);

    double compute(double x) const override;

    std::string getName() const override;
};