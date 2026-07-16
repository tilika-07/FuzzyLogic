#pragma once
#include <memory>
#include "Antecedent.h"
#include "SugenoConsequent.h"
class SugenoRule
{
private:
    Antecedent antecedent_;
    std::shared_ptr<SugenoConsequent> consequent_;

public:
    SugenoRule() = default;

    SugenoRule(
        const Antecedent& antecedent,
        std::shared_ptr<SugenoConsequent> consequent
    );

    const Antecedent& getAntecedent() const;

    const SugenoConsequent& getConsequent() const;

    std::shared_ptr<SugenoConsequent> getConsequentPtr() const;
};