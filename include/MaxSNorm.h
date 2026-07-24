#pragma once
#include "SNorm.h"
class MaxSNorm:public SNorm{
    public:
    double apply(double a,double b) const override;
};