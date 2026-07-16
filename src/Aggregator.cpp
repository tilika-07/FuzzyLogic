#include "../include/Aggregator.h"
#include <unordered_map>
#include <stdexcept>
namespace
{constexpr std::size_t SAMPLE_COUNT = 500;
}
AggregationRes Aggregator::aggregate(const std::vector<Rule>& rules,
    const std::vector<double>& firingStrengths,
    const FuzzyVariable& outputVar,
    const TNorm& implOp,
    const SNorm& aggrOp)const{
    if(rules.size()!=firingStrengths.size()){
        throw std::runtime_error("Size Mismatch Between Rules and Firing Strength");
    }
    const auto& outputSets=outputVar.getSets();
    if(outputSets.empty()){
        throw std::runtime_error("Output variable contains no fuzzy sets");
    }
    const double minVal=outputVar.getMinValue();
    const double maxVal=outputVar.getMaxValue();
    if(maxVal<=minVal){
        throw std::runtime_error("Invalid Output Variable Range");
    }
    const double step=(maxVal-minVal)/static_cast<double>(SAMPLE_COUNT-1);
    std::vector<double> xValues;
    std::vector<double> memberships;
    xValues.reserve(SAMPLE_COUNT);
    memberships.reserve(SAMPLE_COUNT);
    std::unordered_map<std::string,const FuzzySet*> setLookup;
    for(const auto& set:outputSets){
        setLookup[set.getName()]=&set;}
    for(std::size_t sample=0;sample<SAMPLE_COUNT;++sample){
        const double x=minVal+sample*step;
        xValues.push_back(x);
        double aggregatedMembership=0.0;
        bool firstContr=true;
        for(std::size_t ruleIdx=0;ruleIdx<rules.size();++ruleIdx){
            const double firingStr=firingStrengths[ruleIdx];
            const auto& consequentCondtns=rules[ruleIdx].getConsequent().getConditions();
            for(const auto& cndtn:consequentCondtns){
                auto setIt=setLookup.find(cndtn.setName);
                if(setIt==setLookup.end()){continue;}
                const auto& fuzzySet=setIt->second;
                 const double consequentMembership=fuzzySet->membership(x);
                 const double impliedMembership =implOp.apply(firingStr,consequentMembership);
                if(firstContr){
                    aggregatedMembership=impliedMembership;
                    firstContr=false;
                }
                else{
                    aggregatedMembership=aggrOp.apply(aggregatedMembership,impliedMembership);
                }
            }
        }
        memberships.push_back(aggregatedMembership);

    }
    return {
        std::move(xValues),std::move(memberships)
    };

}