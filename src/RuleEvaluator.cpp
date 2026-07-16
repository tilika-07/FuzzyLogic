#include "../include/RuleEvaluator.h"
#include <algorithm>
#include <stdexcept>

double RuleEvaluator::evaluate(
    const Rule& rule,
    const std::unordered_map<std::string, InputValue>& inputs,
    const std::unordered_map<
        std::string,
        std::shared_ptr<FuzzyVariable>
    >& variables,
    const TNorm& tNorm,
    const SNorm& sNorm
) const
{
    return evaluateAntecedent(
        rule.getAntecedent(),
        inputs,
        variables,
        tNorm,
        sNorm
    );
}

double RuleEvaluator::evaluateAntecedent(
    const Antecedent& antecedent,
    const std::unordered_map<
        std::string,
        InputValue
    >& inputs,
    const std::unordered_map<
        std::string,
        std::shared_ptr<FuzzyVariable>
    >& variables,
    const TNorm& tNorm,
    const SNorm& sNorm
) const
{
    const auto& conditions =
        antecedent.getConditions();

    if (conditions.empty())
    {
        throw std::runtime_error(
            "Antecedent contains no conditions."
        );
    }

    std::vector<double> memberships;

    for (const auto& condition : conditions)
    {
        auto inputIt =
            inputs.find(
                condition.variableName
            );

        if (inputIt == inputs.end())
        {
            throw std::runtime_error(
                "Missing input value for variable: "
                + condition.variableName
            );
        }

        auto variableIt =
            variables.find(
                condition.variableName
            );

        if (variableIt == variables.end())
        {
            throw std::runtime_error(
                "Unknown fuzzy variable: "
                + condition.variableName
            );
        }

        const InputValue& input =
            inputIt->second;

        const auto& fuzzyVariable =
            variableIt->second;

        double membership = 0.0;

        if (input.getType() ==
            InputValue::Type::Crisp)
        {
            const FuzzySet* fuzzySet =
                fuzzyVariable->getSet(
                    condition.setName
                );

            if (!fuzzySet)
            {
                throw std::runtime_error(
                    "Unknown fuzzy set: "
                    + condition.setName
                );
            }

            membership =
                fuzzySet->membership(
                    input.getCrispValue()
                );
        }
        else if (
            input.getType() ==
            InputValue::Type::Linguistic)
        {
            membership =
                (
                    input.getLinguisticValue()
                    ==
                    condition.setName
                )
                ? 1.0
                : 0.0;
        }
        else
        {
            const auto& fuzzyMap =
                input.getFuzzyValue();

            auto fuzzyIt =
                fuzzyMap.find(
                    condition.setName
                );

            membership =
                (
                    fuzzyIt !=
                    fuzzyMap.end()
                )
                ? fuzzyIt->second
                : 0.0;
        }

        memberships.push_back(
            membership
        );
    }

    auto op =
        antecedent.getOperator();

    double firingStrength =
        memberships.front();

    if (op == LogicalOperator::AND)
    {
        for (
            size_t i = 1;
            i < memberships.size();
            ++i
        )
        {
            firingStrength =
                tNorm.apply(
                    firingStrength,
                    memberships[i]
                );
        }
    }
    else
    {
        for (
            size_t i = 1;
            i < memberships.size();
            ++i
        )
        {
            firingStrength =
                sNorm.apply(
                    firingStrength,
                    memberships[i]
                );
        }
    }

    return firingStrength;
}