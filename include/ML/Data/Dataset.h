#pragma once
#include <string>
#include <vector>
#include "ML/Types.h"

class Dataset
{
private:

    Matrix data;

    std::vector<std::string> featureNames;

    std::vector<std::string> labels;

public:

    Dataset() = default;

    Dataset(
        const Matrix& data,
        const std::vector<std::string>& featureNames = {},
        const std::vector<std::string>& labels = {}
    );

    // Access entire dataset
    const Matrix& getData() const;

    void setData(const Matrix& newData);

    // Information
    size_t getNumSamples() const;

    size_t getNumFeatures() const;

    // Feature names
    const std::vector<std::string>&
        getFeatureNames() const;
    const std::string& getFeatureName(size_t index) const;

    void setFeatureNames(
        const std::vector<std::string>& names
    );

    // Labels
    const std::vector<std::string>&
        getLabels() const;

    void setLabels(
        const std::vector<std::string>& labels
    );

    // Column extraction
    Vector getFeature(
        size_t index
    ) const;

    Vector getFeature(
        const std::string& featureName
    ) const;

    // Row extraction
    Vector getSample(
        size_t index
    ) const;
};