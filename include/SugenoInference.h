#pragma once
#include <vector>
#include <memory>
#include <unordered_map>
#include <string>
#include "SugenoRule.h"
#include "RuleEvaluator.h"
#include "FuzzyVariable.h"
#include "InputValue.h"

class SugenoInference
{
private:
    RuleEvaluator evaluator_;
    std::shared_ptr<TNorm> tNorm_;
    std::shared_ptr<SNorm> sNorm_;
public:
    explicit SugenoInference(
        const RuleEvaluator& evaluator,
        std::shared_ptr<TNorm> tNorm_,std::shared_ptr<SNorm> sNorm_
    );

    double evaluate(
        const std::vector<SugenoRule>& rules,
        const std::unordered_map<std::string,InputValue>& inputs,
        const std::unordered_map<std::string,std::shared_ptr<FuzzyVariable>>& variables
    ) const;
};