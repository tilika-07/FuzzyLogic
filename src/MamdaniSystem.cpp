#include "../include/MamdaniSystem.h"

#include <stdexcept>

MamdaniSystem::MamdaniSystem(
    std::shared_ptr<InferenceEngine> engine
)
    : engine_(std::move(engine))
{
}

void MamdaniSystem::addRule(
    const Rule& rule
)
{
    rules_.push_back(rule);
}

void MamdaniSystem::removeRule(
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

void MamdaniSystem::clearRules()
{
    rules_.clear();
}

const std::vector<Rule>&
MamdaniSystem::getRules() const
{
    return rules_;
}

std::size_t
MamdaniSystem::ruleCount() const
{
    return rules_.size();
}

void MamdaniSystem::setInferenceEngine(
    std::shared_ptr<InferenceEngine> engine
)
{
    engine_ = std::move(engine);
}

const std::shared_ptr<InferenceEngine>&
MamdaniSystem::getInferenceEngine() const
{
    return engine_;
}

void MamdaniSystem::validate() const
{
    validateCommon();

    if (!engine_)
    {
        throw std::runtime_error(
            "Inference engine not set."
        );
    }

    if (rules_.empty())
    {
        throw std::runtime_error(
            "No rules defined."
        );
    }
}
double MamdaniSystem::evaluate(
    const std::unordered_map<
    std::string,
    InputValue
    >& inputs,
    const std::string& outputVariable
) const
{
    validate();

    auto output =
        getOutputVar(outputVariable);

    if (!output)
    {
        throw std::runtime_error(
            "Unknown output variable: "
            + outputVariable
        );
    }

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

    return engine_->infer(
        rules_,
        inputs,
        variables,
        *output
    );
}