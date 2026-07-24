#pragma once

#include <memory>
#include <vector>
#include <unordered_map>
#include <string>
#include "MinTNorm.h"
#include "MaxSNorm.h"
#include "CentroidDefuzz.h"
#include "FuzzySystem.h"
#include "SugenoInference.h"
#include "SugenoRule.h"
#include "InputValue.h"

class SugenoSystem : public FuzzySystem
{
private:

    std::vector<SugenoRule> rules_;

    std::shared_ptr<SugenoInference> engine_;

public:

    SugenoSystem();

    explicit SugenoSystem(
        std::shared_ptr<SugenoInference> engine
    );

   //rules

    void addRule(
        const SugenoRule& rule
    );

    void removeRule(
        std::size_t index
    );

    void clearRules();

    const std::vector<SugenoRule>&
        getRules() const;

    std::size_t ruleCount() const;

    void setInferenceEngine(
        std::shared_ptr<SugenoInference> engine
    );

    const std::shared_ptr<SugenoInference>&
        getInferenceEngine() const;


    void validate() const override;

   
    double evaluate(
        const std::unordered_map<
        std::string,
        InputValue
        >& inputs
    ) const;
};