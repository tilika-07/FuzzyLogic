#include "../include/Rule.h"

Rule::Rule(
    const Antecedent& antecedent,
    const Consequent& consequent)
    : antecedent_(antecedent),
      consequent_(consequent)
{
}

const Antecedent&
Rule::getAntecedent() const
{
    return antecedent_;
}

const Consequent&
Rule::getConsequent() const
{
    return consequent_;
}