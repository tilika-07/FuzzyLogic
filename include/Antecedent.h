#pragma once
#include <vector>
#include "Condition.h"
enum class LogicalOperator
{AND,OR};

class Antecedent
{
private:
    std::vector<Condition> conditions_;
    LogicalOperator op_;

public:
    Antecedent();

    Antecedent(const std::vector<Condition>& conditions,
               LogicalOperator op = LogicalOperator::AND);      //defaultmlogical op &&

    void addCondition(const Condition& condition);

    const std::vector<Condition>& getConditions() const;

    LogicalOperator getOperator() const;
};