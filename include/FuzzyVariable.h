#pragma once

#include <vector>
#include <string>

#include "FuzzySet.h"

class FuzzyVariable
{
private:
    std::string name_;
    double minValue_;
    double maxValue_;
    std::vector<FuzzySet> sets_;

public:
    explicit FuzzyVariable(
        const std::string& name,double minValue,double maxValue);

    void addSet(FuzzySet set);

    const std::vector<FuzzySet>& getSets() const;
    
    const FuzzySet* getSet(const std::string& setName) const;
    double getMinValue() const;
    double getMaxValue() const;

    const std::string&
    getName() const;
    size_t getNumSets() const;
};
