#include "..\..\..\include\ML\Preprocessing\StandardScaler.h"
void StandardScaler::fit(const Matrix& data)
{
    if (data.empty())
        throw std::invalid_argument("Empty dataset.");
    size_t features = data[0].size();
    if (features == 0)
    {
        throw std::invalid_argument("Dataset contains no features." );
    }

    for (const auto& row : data)
    {
        if (row.size() != features)
        {
            throw std::invalid_argument("All rows must have the same number of features.");
        }
    }
    means.assign(features, 0.0);
    stdDevs.assign(features, 0.0);
    //mean computation
    for (const auto& row : data)
    {
        for (size_t j = 0; j < features; j++)
            means[j] += row[j];
    }
    for (int j = 0; j < features; j++)
        means[j] /= data.size();
    //variance
    for (const auto& row : data)
    {
        for (size_t j = 0; j < features; j++)
        {
            double diff = row[j] - means[j];
            stdDevs[j] += diff * diff;
        }
    }
    for (size_t j = 0; j < features; j++)
    {
        stdDevs[j] = std::sqrt(stdDevs[j] / data.size());

        if (stdDevs[j] == 0)
            stdDevs[j] = 1.0;
    }
    fitted = true;
}
Matrix StandardScaler::transform(const Matrix& data)const{
    if (!fitted)
    {
        throw std::runtime_error( "StandardScaler must be fitted before calling transform.");
    }
    if (data.empty())
    {
        return {};
    }
    for (const auto& row : data)
    {
        if (row.size() != means.size())
        {
            throw std::invalid_argument(
                "Feature dimension does not match fitted scaler."
            );
        }
    }
    Matrix result = data;
    for (auto& row : result){
        for (size_t j = 0; j < row.size(); j++){
            row[j] = (row[j] - means[j]) / stdDevs[j];
        }
    }
    return result;
}
Vector StandardScaler::transform(const Vector& point)const
{
    if (!fitted)
    {
        throw std::runtime_error("StandardScaler must be fitted before calling transform.");
    }
    if (point.size() != means.size())
    {
        throw std::invalid_argument(
            "Feature dimension does not match fitted scaler."
        );
    }

    if (point.size() != means.size())
    {
        throw std::invalid_argument(
            "Feature dimension does not match fitted scaler."
        );
    }
    Vector result = point;
    for (size_t i = 0; i < result.size(); i++)
    {
        result[i] =
            (result[i] - means[i]) / stdDevs[i];
    }

    return result;
}
Matrix StandardScaler::fitTransform(const Matrix& data){
    fit(data);
    return transform(data);
}
Vector StandardScaler::getMeans() const {
    return means;
}
Vector StandardScaler::getStdDevs() const {
    return stdDevs;
}