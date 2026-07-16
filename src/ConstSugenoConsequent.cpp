#include "../include/ConstSugenoConsequent.h"
ConstSugenoConsequent::ConstSugenoConsequent(double val):
    val_(val){}
double ConstSugenoConsequent::evaluate(const std::unordered_map<std::string,double>& inputs) const{
    return val_;
}
double ConstSugenoConsequent::getVal() const{
    return val_;
}