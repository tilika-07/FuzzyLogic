#pragma once

#include "Antecedent.h"
#include "Consequent.h"

class Rule
{
private:
    Antecedent antecedent_;
    Consequent consequent_;

public:
    Rule() = default;

    Rule(const Antecedent& antecedent,
         const Consequent& consequent);

    const Antecedent& getAntecedent() const;

    const Consequent& getConsequent() const;
};