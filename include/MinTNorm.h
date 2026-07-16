#pragma once
#include "TNorm.h"
class MinTNorm:public TNorm{
    public:
        double apply(double a,double b) const override;
};