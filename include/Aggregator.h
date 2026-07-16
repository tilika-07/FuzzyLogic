#pragma once
#include <vector>
#include <utility>
#include "Rule.h"
#include "TNorm.h"
#include "SNorm.h"
#include "FuzzyVariable.h"
#include "FuzzySet.h"
using AggregationRes=std::pair<std::vector<double>,std::vector<double>>;        //only for mamdani
class Aggregator{
    public:
    AggregationRes aggregate(const std::vector<Rule>& rules,
        const std::vector<double>& firingStrengths,const FuzzyVariable& outputVar,
        const TNorm& implOp, const SNorm& aggrOp) const;
    private:
        static constexpr std::size_t SAMPLE_COUNT=500;
};