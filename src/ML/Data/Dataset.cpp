#include "ML/Data/Dataset.h"

#include <stdexcept>

Dataset::Dataset(
    const Matrix& data,
    const std::vector<std::string>& featureNames,
    const std::vector<std::string>& labels
)
    :
    data(data),
    featureNames(featureNames),
    labels(labels)
{
}

const Matrix&
Dataset::getData() const
{
    return data;
}

void Dataset::setData(
    const Matrix& newData
)
{
    data = newData;
}

size_t Dataset::getNumSamples() const
{
    return data.size();
}

size_t Dataset::getNumFeatures() const
{
    if (data.empty())
        return 0;

    return data[0].size();
}

const std::vector<std::string>&
Dataset::getFeatureNames() const
{
    return featureNames;
}

void Dataset::setFeatureNames(
    const std::vector<std::string>& names
)
{
    featureNames = names;
}

const std::vector<std::string>&
Dataset::getLabels() const
{
    return labels;
}

void Dataset::setLabels(
    const std::vector<std::string>& newLabels
)
{
    labels = newLabels;
}

Vector Dataset::getFeature(
    size_t index
) const
{
    if (index >= getNumFeatures())
    {
        throw std::out_of_range(
            "Feature index out of range."
        );
    }

    Vector column;

    for (const auto& row : data)
    {
        column.push_back(row[index]);
    }

    return column;
}

Vector Dataset::getFeature(
    const std::string& featureName
) const
{
    for (size_t i = 0; i < featureNames.size(); i++)
    {
        if (featureNames[i] == featureName)
        {
            return getFeature(i);
        }
    }

    throw std::invalid_argument(
        "Unknown feature name."
    );
}

Vector Dataset::getSample(
    size_t index
) const
{
    if (index >= getNumSamples())
    {
        throw std::out_of_range(
            "Sample index out of range."
        );
    }

    return data[index];
}
const std::string& Dataset::getFeatureName(size_t index) const
{
    if (index >= featureNames.size())
    {
        throw std::out_of_range(
            "Feature index out of range."
        );
    }

    return featureNames[index];
}