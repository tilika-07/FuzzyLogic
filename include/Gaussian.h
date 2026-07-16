#pragma once
#include "MembershipFunc.h"
class Gaussian:public MembershipFunc{
    private:
        std::string name_;
        double mean_;
        double sigma_;
    public:
        Gaussian(const std::string& name,double mean,double sigma);
        double compute(double x) const override;
        std::string getName() const override;
        double getMean() const;
        double getSigma() const;
        
};