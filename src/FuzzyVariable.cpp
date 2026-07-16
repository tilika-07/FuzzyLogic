#include "../include/FuzzyVariable.h"
FuzzyVariable::FuzzyVariable(const std::string&name,double minValue,double maxValue):
    name_(name),
    minValue_(minValue),
    maxValue_(maxValue){}
void FuzzyVariable::addSet(FuzzySet set){
    sets_.push_back(std::move(set));
}
const std::vector<FuzzySet>& FuzzyVariable::getSets() const{
    return sets_;
}
const FuzzySet*
FuzzyVariable::getSet(const std::string& setName) const{
    for(const auto& set : sets_)
    {if(set.getName() == setName)
        {return &set;}
    }
    return nullptr;
}
const std::string& FuzzyVariable::getName() const{
    return name_;
}
double FuzzyVariable::getMinValue() const
{
    return minValue_;
}

double FuzzyVariable::getMaxValue() const
{
    return maxValue_;
}
size_t FuzzyVariable::getNumSets() const
{
    return sets_.size();
}
