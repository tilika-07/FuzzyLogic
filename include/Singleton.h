#pragma once

#include "MembershipFunc.h"

class Singleton : public MembershipFunc
{
private:
    std::string name_;

    double center_;

public:
    Singleton(
        const std::string& name,
        double center);

    double compute(double x) const override;

    std::string getName() const override;
};