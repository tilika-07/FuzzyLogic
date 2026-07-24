#include "ML/Clustering/ClusteringMetrics.h"
#include <algorithm>
#include <stdexcept>
#include <unordered_map>

double ClusteringMetrics::purity(
    const Matrix& memberships,
    const std::vector<std::string>& labels
)
{
    //valdtn

    if (memberships.empty())
    {
        throw std::invalid_argument(
            "Membership matrix is empty."
        );
    }

    if (labels.empty())
    {
        throw std::invalid_argument(
            "Label vector is empty."
        );
    }

    if (memberships.size() != labels.size())
    {
        throw std::invalid_argument(
            "Membership matrix and label vector must have the same number of samples."
        );
    }

    const size_t numClusters =
        memberships.front().size();

    if (numClusters == 0)
    {
        throw std::invalid_argument(
            "Membership matrix contains no clusters."
        );
    }
    //num labels
    std::vector<
        std::unordered_map<std::string, int>
    > clusterCounts(numClusters);

    for (size_t sample = 0;
        sample < memberships.size();
        ++sample)
    {
        const auto& membership =
            memberships[sample];

        if (membership.size() != numClusters)
        {
            throw std::invalid_argument(
                "Membership matrix is not rectangular."
            );
        }

        auto maxMembership =
            std::max_element(
                membership.begin(),
                membership.end()
            );

        size_t cluster =
            std::distance(
                membership.begin(),
                maxMembership
            );

        clusterCounts[cluster][labels[sample]]++;
    }

  //calc purity

    int correctlyClustered = 0;

    for (const auto& cluster : clusterCounts)
    {
        int majorityCount = 0;

        for (const auto& entry : cluster)
        {
            majorityCount =
                std::max(
                    majorityCount,
                    entry.second
                );
        }

        correctlyClustered += majorityCount;
    }

    return static_cast<double>(correctlyClustered)
        / memberships.size();
}