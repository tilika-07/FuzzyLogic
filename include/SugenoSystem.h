#pragma once

#include <memory>
#include <vector>
#include <unordered_map>
#include <string>

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

    SugenoSystem() = default;

    explicit SugenoSystem(
        std::shared_ptr<SugenoInference> engine
    );

    //--------------------------------------------------
    // Rule Management
    //--------------------------------------------------

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

    //--------------------------------------------------
    // Engine
    //--------------------------------------------------

    void setInferenceEngine(
        std::shared_ptr<SugenoInference> engine
    );

    const std::shared_ptr<SugenoInference>&
        getInferenceEngine() const;

    //--------------------------------------------------
    // Validation
    //--------------------------------------------------

    void validate() const override;

    //--------------------------------------------------
    // Evaluation
    //--------------------------------------------------

    double evaluate(
        const std::unordered_map<
        std::string,
        InputValue
        >& inputs
    ) const;
};