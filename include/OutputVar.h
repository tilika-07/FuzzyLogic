#pragma once
#include "FuzzyVariable.h"
class OutputVar:public FuzzyVariable{
    public:
    explicit OutputVar(const std::string& name,double minValue,double maxValue);
};