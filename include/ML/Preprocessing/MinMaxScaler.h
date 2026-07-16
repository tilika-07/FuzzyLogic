#pragma once
#include <vector>
#include <stdexcept>
#include <cmath>
using Vector = std::vector<double>;
using Matrix = std::vector<Vector>;
class MinMaxScaler
{
private:
    bool fitted = false;
    Vector minimums;
    Vector maximums;
public:
    void fit(const Matrix& data);
    Matrix transform(const Matrix& data) const;
    Matrix fitTransform(const Matrix& data);
    Vector transform(const Vector& point) const;
    Vector getMinimums() const;
    Vector getMaximums() const;
};