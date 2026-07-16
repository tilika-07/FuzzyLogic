#pragma once
#include "SugenoConsequent.h"
class ConstSugenoConsequent:public SugenoConsequent{
    private:
        double val_;
    public:
        explicit ConstSugenoConsequent(double val);
        double evaluate(const std::unordered_map<std::string,double>& inputs) const override;
        double getVal() const;

};