#include "../include/Antecedent.h"

Antecedent::Antecedent()
    : op_(LogicalOperator::AND){}       //default op is AND
Antecedent::Antecedent(
    const std::vector<Condition>& conditions,
    LogicalOperator op)
    : conditions_(conditions),
      op_(op)
{
}

void Antecedent::addCondition(
    const Condition& condition)
{
    conditions_.push_back(condition);
}

const std::vector<Condition>&
Antecedent::getConditions() const
{
    return conditions_;
}

LogicalOperator Antecedent::getOperator() const
{
    return op_;
}