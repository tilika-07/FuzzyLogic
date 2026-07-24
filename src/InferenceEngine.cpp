#include "../include/InferenceEngine.h"
#include <stdexcept>
#include <utility>

InferenceEngine::InferenceEngine(
    std::shared_ptr<TNorm> tNorm,
    std::shared_ptr<SNorm> sNorm,
    std::shared_ptr<Defuzzifier> defuzzifier
)
    : tNorm_(std::move(tNorm)),
    sNorm_(std::move(sNorm)),
    defuzzifier_(std::move(defuzzifier))
{
    if (!tNorm_)
    {
        throw std::invalid_argument(
            "TNorm cannot be null."
        );
    }

    if (!sNorm_)
    {
        throw std::invalid_argument(
            "SNorm cannot be null."
        );
    }

    if (!defuzzifier_)
    {
        throw std::invalid_argument(
            "Defuzzifier cannot be null."
        );
    }
}

double InferenceEngine::infer(
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
) const
{
    if (rules.empty())
    {
        throw std::runtime_error(
            "No rules in inference engine."
        );
    }

    std::vector<double> firingStrengths;
    firingStrengths.reserve(rules.size());

    for (const auto& rule : rules)
    {
        firingStrengths.push_back(
            ruleEvaluator_.evaluate(
                rule,
                inputs,
                variables,
                *tNorm_,
                *sNorm_
            )
        );
    }

    auto aggregated =
        aggregator_.aggregate(
            rules,
            firingStrengths,
            outputVariable,
            *tNorm_,
            *sNorm_
        );

    return defuzzifier_->defuzzify(
        aggregated.first,
        aggregated.second
    );
}

const TNorm&
InferenceEngine::getTNorm() const
{
    return *tNorm_;
}

const SNorm&
InferenceEngine::getSNorm() const
{
    return *sNorm_;
}

const Defuzzifier&
InferenceEngine::getDefuzzifier() const
{
    return *defuzzifier_;
}