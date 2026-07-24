#pragma once
#include <memory>
#include <unordered_map>
#include <string>
#include "RuleEvaluator.h"
#include "Aggregator.h"
#include "Defuzzifier.h"
#include "TNorm.h"
#include "SNorm.h"
#include "InputValue.h"
#include "FuzzyVariable.h"
#include "Rule.h"

class InferenceEngine
{
private:

    RuleEvaluator ruleEvaluator_;
    Aggregator aggregator_;

    std::shared_ptr<TNorm> tNorm_;
    std::shared_ptr<SNorm> sNorm_;
    std::shared_ptr<Defuzzifier> defuzzifier_;

public:

    InferenceEngine(
        std::shared_ptr<TNorm> tNorm,
        std::shared_ptr<SNorm> sNorm,
        std::shared_ptr<Defuzzifier> defuzzifier
    );

    double infer(
        const std::vector<Rule>& rules,
        const std::unordered_map<
        std::string,
        InputValue
        >& inputs,
        const std::unordered_map<
        std::string,
        std::shared_ptr<FuzzyVariable>
        >& variables,
        const FuzzyVariable& outputVariable
    ) const;

    const TNorm& getTNorm() const;

    const SNorm& getSNorm() const;

    const Defuzzifier& getDefuzzifier() const;
};