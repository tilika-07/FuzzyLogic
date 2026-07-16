#pragma once
#include <String>
#include <unordered_map>
class InputValue{
    public:
    enum class Type{
        Crisp,Linguistic,Fuzzy
    };
    private:
    Type type_;
    double crispValue_{};
    std::string linguisticValue_;
    std::unordered_map<std::string,double> fuzzyValue_;
    public:
    InputValue(double value);
    InputValue(const std::string& value);
    InputValue(const std::unordered_map<std::string,double>& value);
    Type getType() const;
    double getCrispValue() const;
    const std::string& getLinguisticValue() const;
    const std::unordered_map<std::string,double>& getFuzzyValue() const;
    
};