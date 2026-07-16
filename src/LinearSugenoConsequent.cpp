#include "../include/LinearSugenoConsequent.h"
LinearSugenoConsequent::LinearSugenoConsequent(
    const std::unordered_map<std::string, double>& coefficients,
    double bias
)
    : coefficients_(coefficients),
      bias_(bias)
{}

double LinearSugenoConsequent::evaluate(const std::unordered_map<std::string, double>& inputs
) const
{
    double result = bias_;

    for(const auto& pair : coefficients_)
    {
        auto inputIt = inputs.find(pair.first);

        if (inputIt != inputs.end())
        {
            result += pair.second * inputIt->second;
        }
    }

    return result;
}

void LinearSugenoConsequent::setCoefficient(
    const std::string& variable,
    double coefficient
){
    coefficients_[variable] = coefficient;
}

double LinearSugenoConsequent::getBias() const
{
    return bias_;
}