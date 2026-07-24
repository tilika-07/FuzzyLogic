#pragma once
#include "../Types.h"
class ClusteringMetrics
{
public:
    static double purity(
        const Matrix& memberships,
        const std::vector<std::string>& labels);
    static Matrix confusionMatrix(
        const Matrix& memberships,
        const std::vector<std::string>& labels
    );
};