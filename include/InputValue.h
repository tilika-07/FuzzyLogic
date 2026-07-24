#pragma once
#include <String>
#include <unordered_map>
class InputValue{
    public:
    enum class Type{
        Crisp,Linguistic,Fuzzy      //user can give either type of input
    };
    private:
    Type type_;
    double crispValue_{};
    std::string linguisticValue_;
    std::unordered_map<std::string,double> fuzzyValue_;
    public:
    InputValue(double value);       //crisp constructor
    InputValue(const std::string& value);       //linguistic constructor
    InputValue(const std::unordered_map<std::string,double>& value);   //fuzzy constructor     
    Type getType() const;
    double getCrispValue() const;
    const std::string& getLinguisticValue() const;
    const std::unordered_map<std::string,double>& getFuzzyValue() const;
    
};