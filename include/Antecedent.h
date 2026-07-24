#pragma once
#include <vector>
#include "Condition.h"
enum class LogicalOperator      //for antecedent constructor
{AND,OR};

class Antecedent
{
private:
    std::vector<Condition> conditions_;     //incase 1 antecedent has mult conditions
    LogicalOperator op_;

public:
    Antecedent();

    Antecedent(const std::vector<Condition>& conditions,
               LogicalOperator op = LogicalOperator::AND);      //default logical op &&

    void addCondition(const Condition& condition);

    const std::vector<Condition>& getConditions() const;

    LogicalOperator getOperator() const;
};