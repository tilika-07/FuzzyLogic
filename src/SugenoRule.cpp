#include "../include/SugenoRule.h"

SugenoRule::SugenoRule(
    const Antecedent& antecedent,
    std::shared_ptr<SugenoConsequent> consequent
)
    : antecedent_(antecedent),
      consequent_(std::move(consequent))
{
}

const Antecedent& SugenoRule::getAntecedent() const
{
    return antecedent_;
}

const SugenoConsequent&
SugenoRule::getConsequent() const
{
    return *consequent_;
}

std::shared_ptr<SugenoConsequent>
SugenoRule::getConsequentPtr() const
{
    return consequent_;
}