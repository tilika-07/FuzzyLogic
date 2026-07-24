#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include "InputVar.h"
#include "OutputVar.h"

class FuzzySystem
{
protected:

    std::unordered_map<
        std::string,
        InputVar
    > inputVars_;

    std::unordered_map<
        std::string,
        OutputVar
    > outputVars_;

public:

    FuzzySystem() = default;

    virtual ~FuzzySystem() = default;
    void addInputVar(
        const InputVar& variable
    );
    void removeInputVar(
        const std::string& name
    );

    bool hasInputVar(
        const std::string& name
    ) const;

    const InputVar* getInputVar(
        const std::string& name
    ) const;

    const std::unordered_map<
        std::string,
        InputVar
    >& getInputVars() const;

    std::size_t inputCount() const;
    void addOutputVar(
        const OutputVar& variable
    );

    void removeOutputVar(
        const std::string& name
    );

    bool hasOutputVar(
        const std::string& name
    ) const;

    const OutputVar* getOutputVar(
        const std::string& name
    ) const;

    const std::unordered_map<
        std::string,
        OutputVar
    >& getOutputVars() const;

    std::size_t outputCount() const;
    virtual void validate() const=0;

    virtual void clear();

    bool empty() const; 
 protected:

       void validateCommon() const;


};