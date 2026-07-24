#include "../include/FuzzySystem.h"
#include <stdexcept>
void FuzzySystem::addInputVar(
    const InputVar& variable
)
{
    auto result =
        inputVars_.emplace(
            variable.getName(),
            variable
        );

    if (!result.second)
    {
        throw std::runtime_error(
            "Input variable already exists: "
            + variable.getName()
        );
    }
}

void FuzzySystem::removeInputVar(
    const std::string& name
)
{
    auto erased =
        inputVars_.erase(name);

    if (erased == 0)
    {
        throw std::runtime_error(
            "Unknown input variable: "
            + name
        );
    }
}

bool FuzzySystem::hasInputVar(
    const std::string& name
) const
{
    return inputVars_.find(name)
        != inputVars_.end();
}

const InputVar*
FuzzySystem::getInputVar(
    const std::string& name
) const
{
    auto it =
        inputVars_.find(name);

    if (it == inputVars_.end())
    {
        return nullptr;
    }

    return &it->second;
}

const std::unordered_map<
    std::string,
    InputVar
>&
FuzzySystem::getInputVars() const
{
    return inputVars_;
}

std::size_t
FuzzySystem::inputCount() const
{
    return inputVars_.size();
}



void FuzzySystem::addOutputVar(
    const OutputVar& variable
)
{
    auto result =
        outputVars_.emplace(
            variable.getName(),
            variable
        );

    if (!result.second)
    {
        throw std::runtime_error(
            "Output variable already exists: "
            + variable.getName()
        );
    }
}

void FuzzySystem::removeOutputVar(
    const std::string& name
)
{
    auto erased =
        outputVars_.erase(name);

    if (erased == 0)
    {
        throw std::runtime_error(
            "Unknown output variable: "
            + name
        );
    }
}

bool FuzzySystem::hasOutputVar(
    const std::string& name
) const
{
    return outputVars_.find(name)
        != outputVars_.end();
}

const OutputVar*
FuzzySystem::getOutputVar(
    const std::string& name
) const
{
    auto it =
        outputVars_.find(name);

    if (it == outputVars_.end())
    {
        return nullptr;
    }

    return &it->second;
}

const std::unordered_map<
    std::string,
    OutputVar
>&
FuzzySystem::getOutputVars() const
{
    return outputVars_;
}

std::size_t
FuzzySystem::outputCount() const
{
    return outputVars_.size();
}



void FuzzySystem::validate() const
{
    if (inputVars_.empty())
    {
        throw std::runtime_error(
            "Fuzzy system contains no input variables."
        );
    }

    if (outputVars_.empty())
    {
        throw std::runtime_error(
            "Fuzzy system contains no output variables."
        );
    }
}


void FuzzySystem::clear()
{
    inputVars_.clear();
    outputVars_.clear();
}

bool FuzzySystem::empty() const
{
    return
        inputVars_.empty() &&
        outputVars_.empty();
}
void FuzzySystem::validateCommon() const
{
    if (inputVars_.empty())
    {
        throw std::runtime_error(
            "No input variables."
        );
    }

    if (outputVars_.empty())
    {
        throw std::runtime_error(
            "No output variables."
        );
    }
}