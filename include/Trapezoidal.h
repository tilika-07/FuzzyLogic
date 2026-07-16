#pragma once
#include "MembershipFunc.h"
class Trapezoidal:public MembershipFunc{
    private:
        std::string name_;
        double a_,b_,c_,d_;
    public:
        Trapezoidal(const std::string& name,double a,double b,double c,double d);
        double compute(double x) const override;
        std::string getName() const override;
        double getA() const;

        double getB() const;

        double getC() const;

        double getD() const;
};