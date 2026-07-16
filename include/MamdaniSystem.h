#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "FuzzySystem.h"
#include "InferenceEngine.h"
#include "Rule.h"
#include "InputValue.h"

class MamdaniSystem : public FuzzySystem
{
private:

    std::vector<Rule> rules_;

    std::shared_ptr<InferenceEngine> engine_;

public:

    MamdaniSystem() = default;

    explicit MamdaniSystem(
        std::shared_ptr<InferenceEngine> engine
    );

    //--------------------------------------------------
    // Rule Management
    //--------------------------------------------------

    void addRule(
        const Rule& rule
    );

    void removeRule(
        std::size_t index
    );

    void clearRules();

    const std::vector<Rule>&
        getRules() const;

    std::size_t ruleCount() const;

    //--------------------------------------------------
    // Engine
    //--------------------------------------------------

    void setInferenceEngine(
        std::shared_ptr<InferenceEngine> engine
    );

    const std::shared_ptr<InferenceEngine>&
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
        >& inputs,
        const std::string& outputVariable
    ) const;
};