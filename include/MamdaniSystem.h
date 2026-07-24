#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include "MinTNorm.h"
#include "MaxSNorm.h"
#include "CentroidDefuzz.h"
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

    MamdaniSystem();

    explicit MamdaniSystem(
        std::shared_ptr<InferenceEngine> engine
    );

    //rules related func

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


    void setInferenceEngine(
        std::shared_ptr<InferenceEngine> engine
    );

    const std::shared_ptr<InferenceEngine>&
        getInferenceEngine() const;


    void validate() const override;


    double evaluate(
        const std::unordered_map<
        std::string,
        InputValue
        >& inputs,
        const std::string& outputVariable
    ) const;
};