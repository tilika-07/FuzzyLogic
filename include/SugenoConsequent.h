#pragma once
#include <unordered_map>
#include <string>
class SugenoConsequent{
    public:
        virtual ~SugenoConsequent()=default;
        virtual double evaluate(const std::unordered_map<std::string,double>& inputs) const=0;
};