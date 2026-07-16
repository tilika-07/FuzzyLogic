#include "../include/InputValue.h"

InputValue::InputValue(double value)
    : type_(Type::Crisp),
      crispValue_(value){}

InputValue::InputValue(
    const std::string& value
)
    : type_(Type::Linguistic),
      linguisticValue_(value)
{
}

InputValue::InputValue(
    const std::unordered_map<
        std::string,
        double
    >& value
)
    : type_(Type::Fuzzy),
      fuzzyValue_(value)
{
}

InputValue::Type
InputValue::getType() const
{
    return type_;
}

double
InputValue::getCrispValue() const
{
    return crispValue_;
}

const std::string&
InputValue::getLinguisticValue() const
{
    return linguisticValue_;
}

const std::unordered_map<
    std::string,
    double
>&
InputValue::getFuzzyValue() const
{
    return fuzzyValue_;
}