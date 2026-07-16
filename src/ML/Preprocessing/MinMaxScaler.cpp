#include "..\..\..\include\ML\Preprocessing\MinMaxScaler.h"
void MinMaxScaler::fit(
    const Matrix& data
)
{
    if (data.empty())
        throw std::invalid_argument("Empty dataset.");
   
    int features = data[0].size();
    if (features == 0)
    {
        throw std::invalid_argument("Dataset contains no features.");

    }
    for (const auto& row : data)
    {
        if (row.size() != features)
        {
            throw std::invalid_argument(
                "All rows must have the same number of features."
            );
        }
    }
    minimums = data[0];
    maximums = data[0];

    for (const auto& row : data)
    {
        for (int j = 0; j < features; j++)
        {
            minimums[j] = std::min(minimums[j], row[j]);
            maximums[j] = std::max(maximums[j], row[j]);
        }
    }

    for (int j = 0; j < features; j++)
    {
        if (minimums[j] == maximums[j])
            maximums[j] = minimums[j] + 1.0;
    }
    fitted = true;
}
Matrix MinMaxScaler::transform(const Matrix& data) const
{
    if (!fitted)
    {
        throw std::runtime_error("MinMaxScaler must be fitted before calling transform.");
    }
    if (data.empty())
    {
        return {};
    }
    for (const auto& row : data)
    {
        if (row.size() != minimums.size())
        {
            throw std::invalid_argument(
                "Feature dimension does not match fitted scaler."
            );
        }
    }
    Matrix result = data;

    for (auto& row : result)
    {
        for (int j = 0; j < row.size(); j++)
        {
            row[j] =
                (row[j] - minimums[j]) /
                (maximums[j] - minimums[j]);
        }
    }

    return result;
}
Vector MinMaxScaler::transform(const Vector& point)const
{
    if (!fitted)
    {
        throw std::runtime_error("MinMaxScaler must be fitted before calling transform.");
    }
    if (point.size() != minimums.size())
    {
        throw std::invalid_argument(
            "Feature dimension does not match fitted scaler."
        );
    }
    Vector result = point;

    for (int i = 0; i < result.size(); i++)
    {
        result[i] =
            (result[i] - minimums[i]) /
            (maximums[i] - minimums[i]);
    }

    return result;
}
Vector MinMaxScaler::getMinimums() const
{
  return minimums;
}
Vector MinMaxScaler::getMaximums() const
{
    return maximums;
}
Matrix MinMaxScaler::fitTransform(
    const Matrix& data
)
{
    fit(data);
    return transform(data);
}
