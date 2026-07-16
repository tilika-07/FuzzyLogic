#pragma once
#include "FuzzyVariable.h"
class InputVar:public FuzzyVariable{
    public:
    explicit InputVar(const std:: string& name,double minValue, double maxValue);
};