#pragma once
#include "MembershipFunc.h"
class Triangle:public MembershipFunc{
    private:
        std::string name_;
        double a_,b_,c_;
    public:
        Triangle(
            const std::string& name,
            double a,       //left endpoint
            double b,       //peak         
            double c        //right endpoint
        );
        double compute(double x) const override;
        double getLeft() const;
        double getPeak() const;
        double getRight() const;
        std::string getName() const override;
};