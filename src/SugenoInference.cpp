#include "../include/SugenoInference.h"

#include <stdexcept>

SugenoInference::SugenoInference(
    const RuleEvaluator& evaluator,
    std::shared_ptr<TNorm> tNorm,
    std::shared_ptr<SNorm> sNorm
)
    : evaluator_(evaluator),
      tNorm_(std::move(tNorm)),
      sNorm_(std::move(sNorm))
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
}

double SugenoInference::evaluate(
    const std::vector<SugenoRule>& rules,
    const std::unordered_map<
        std::string,
        InputValue
    >& inputs,
    const std::unordered_map<
        std::string,
        std::shared_ptr<FuzzyVariable>
    >& variables
) const
{
    std::unordered_map<
        std::string,
        double
    > crispInputs;

    for (const auto& [name, value] : inputs)
    {
        if (
            value.getType()
            != InputValue::Type::Crisp
        )
        {
            throw std::runtime_error(
                "Sugeno consequents require crisp inputs."
            );
        }

        crispInputs[name] =
            value.getCrispValue();
    }

    double weightedSum = 0.0;
    double firingSum = 0.0;
    if (rules.empty())
    {
        throw std::runtime_error("No Sugeno rules.");
    }
    for (const auto& rule : rules)
    {
        double firingStrength =
            evaluator_.evaluateAntecedent(
                rule.getAntecedent(),
                inputs,
                variables,
                *tNorm_,
                *sNorm_
            );

        double output =
            rule.getConsequent().evaluate(
                crispInputs
            );

        weightedSum +=
            firingStrength * output;

        firingSum +=
            firingStrength;
    }

    if (firingSum == 0.0)
    {
        throw std::runtime_error(
            "All Sugeno rules fired with zero strength."
        );
    }

    return weightedSum / firingSum;
}