#pragma once
#include <vector>
#include "Condition.h"
class Consequent
{
private:
    std::vector<Condition> conditions_;

public:
    Consequent();

    Consequent(const std::vector<Condition>& conditions);

    void addCondition(const Condition& condition);

    const std::vector<Condition>& getConditions() const;
};