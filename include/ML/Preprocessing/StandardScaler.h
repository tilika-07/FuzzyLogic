#pragma once
#include <vector>
#include <stdexcept>
#include <cmath>
using Vector = std::vector<double>;
using Matrix = std::vector<Vector>;
class StandardScaler
{
private:
    Vector means;
    Vector stdDevs;
public:
    bool fitted = false;
    void fit(const Matrix& data);
    Matrix transform(const Matrix& data) const;
    Matrix fitTransform(const Matrix& data);
    Vector transform(const Vector& point) const;
    Vector getMeans() const;
    Vector getStdDevs() const;
};