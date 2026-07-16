#pragma once
#include "SugenoConsequent.h"
#include <unordered_map>
#include <string>

class LinearSugenoConsequent : public SugenoConsequent{
private:
    std::unordered_map<std::string, double> coefficients_;
    double bias_;
public:
    LinearSugenoConsequent(
        const std::unordered_map<std::string, double>& coefficients,
        double bias = 0.0
    );

    double evaluate(const std::unordered_map<std::string, double>& inputs) const override;

    void setCoefficient(
        const std::string& variable,
        double coefficient
    );
    double getBias() const;
};