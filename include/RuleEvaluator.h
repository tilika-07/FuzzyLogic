#pragma once
#include <memory>
#include "FuzzyVariable.h"
#include "Rule.h"
#include "TNorm.h"
#include "SNorm.h"
#include "InputValue.h"
#include <unordered_map>

class RuleEvaluator{
public:
    RuleEvaluator()=default;
    double evaluate(
    const Rule& rule,
    const std::unordered_map<std::string,InputValue>& inputs,
    const std::unordered_map<std::string,std::shared_ptr<FuzzyVariable>>& variables,
    const TNorm& tNorm,
    const SNorm& sNorm
    ) const;
    
   double evaluateAntecedent(
    const Antecedent& antecedent,
    const std::unordered_map<std::string,InputValue>& inputs,
    const std::unordered_map<std::string,std::shared_ptr<FuzzyVariable>>& variables,
    const TNorm& tNorm,
    const SNorm& sNorm
) const;
};