#include "../include/Consequent.h"
Consequent::Consequent(){}
Consequent::Consequent(
    const std::vector<Condition>& conditions)
    : conditions_(conditions){}
void Consequent::addCondition(const Condition& condition){
    conditions_.push_back(condition);
}
const std::vector<Condition>& Consequent::getConditions() const{
    return conditions_;
}