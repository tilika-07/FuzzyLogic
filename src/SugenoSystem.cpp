#include "../include/SugenoSystem.h"

#include <stdexcept>

SugenoSystem::SugenoSystem(
    std::shared_ptr<SugenoInference> engine
)
    : engine_(std::move(engine))
{
}

void SugenoSystem::addRule(
    const SugenoRule& rule
)
{
    rules_.push_back(rule);
}

void SugenoSystem::removeRule(
    std::size_t index
)
{
    if (index >= rules_.size())
    {
        throw std::out_of_range(
            "Invalid rule index."
        );
    }

    rules_.erase(
        rules_.begin() + index
    );
}

void SugenoSystem::clearRules()
{
    rules_.clear();
}

const std::vector<SugenoRule>&
SugenoSystem::getRules() const
{
    return rules_;
}

std::size_t
SugenoSystem::ruleCount() const
{
    return rules_.size();
}

void SugenoSystem::setInferenceEngine(
    std::shared_ptr<SugenoInference> engine
)
{
    engine_ = std::move(engine);
}

const std::shared_ptr<SugenoInference>&
SugenoSystem::getInferenceEngine() const
{
    return engine_;
}

void SugenoSystem::validate() const
{
    if (inputVars_.empty())
    {
        throw std::runtime_error(
            "No input variables."
        );
    }

    if (!engine_)
    {
        throw std::runtime_error(
            "Sugeno inference engine not set."
        );
    }

    if (rules_.empty())
    {
        throw std::runtime_error(
            "No Sugeno rules defined."
        );
    }
}
double SugenoSystem::evaluate(
    const std::unordered_map<
    std::string,
    InputValue
    >& inputs
) const
{
    validate();

    std::unordered_map<
        std::string,
        std::shared_ptr<FuzzyVariable>
    > variables;

    for (const auto& pair : getInputVars())
    {
        variables.emplace(
            pair.first,
            std::make_shared<InputVar>(
                pair.second
                )
        );
    }

    return engine_->evaluate(
        rules_,
        inputs,
        variables
    );
}