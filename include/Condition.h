#pragma once
#include <string>
struct Condition
{
    std::string variableName;
    std::string setName;

    Condition() = default;

    Condition(const std::string& variable,
              const std::string& set)
        : variableName(variable),
          setName(set)
    {
    }
};