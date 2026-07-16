#include "ML/Generation/FuzzySetGenerator.h"
#include "ML/Clustering/FuzzyCMeans.h"
#include "Gaussian.h"
#include "Triangle.h"
#include "Trapezoidal.h"
#include <algorithm>
#include <cmath>
FuzzySetGenerator::ClusterInformation       //necessary steps for each membership func
FuzzySetGenerator::computeClusterInformation(
    const Dataset& dataset,
    size_t featureIndex,
    int numSets,
    double fuzziness
)
{
    if (featureIndex >= dataset.getNumFeatures())
    {
        throw std::out_of_range(
            "Feature index out of range.");
    }
    ClusterInformation info;
    info.featureData =dataset.getFeature(featureIndex);
    Matrix trainingData;
    trainingData.reserve( info.featureData.size());
    for (double value : info.featureData)
    { trainingData.push_back({ value });}
    //getting clusters from fcm
    FuzzyCMeans fcm(numSets,fuzziness);
    fcm.fit(trainingData);
    Matrix centerMatrix =fcm.getCenters();
    info.clusters.reserve(centerMatrix.size());
    for (size_t i = 0; i < centerMatrix.size(); i++)
    {
        info.clusters.push_back(
            {
                centerMatrix[i][0],i
            });
    }
    std::sort(
        info.clusters.begin(),
        info.clusters.end(),
        [](const Cluster& a,
            const Cluster& b)
        {
            return a.center < b.center;
        }
    );
    info.memberships =fcm.getMemberships();

    if (!dataset.getFeatureNames().empty())
    {
        info.featureName = dataset.getFeatureName(featureIndex);
    }
    else
    {
        info.featureName = "Feature_" +std::to_string(featureIndex);
    }
//range
    info.minimum =
        *std::min_element(
            info.featureData.begin(),
            info.featureData.end()
        );

    info.maximum =
        *std::max_element(
            info.featureData.begin(),
            info.featureData.end()
        );

    return info;
}
std::vector<std::shared_ptr<FuzzySet>>
FuzzySetGenerator::generateGaussianSets(
    const Dataset& dataset,
    size_t featureIndex,
    int numSets,
    double fuzziness
)
{
    auto info =
        computeClusterInformation(
            dataset,
            featureIndex,
            numSets,
            fuzziness
        );
    Vector sigmas =
        computeSigmas(
            info.featureData,
            info.clusters,
            info.memberships,
            fuzziness
        );

    std::vector<std::shared_ptr<FuzzySet>> fuzzySets;

    fuzzySets.reserve(numSets);

    for (int i = 0; i < numSets; i++)
    {
        fuzzySets.push_back(
            std::make_shared<FuzzySet>(
                info.featureName + "_" + std::to_string(i + 1),

                std::make_shared<Gaussian>(
                    info.featureName + "_" + std::to_string(i + 1),
                    info.clusters[i].center,
                    sigmas[i]
                    )
                )
        );
    }

    return fuzzySets;
}
FuzzyVariable
FuzzySetGenerator::generateGaussianVariable(
    const Dataset& dataset,
    size_t featureIndex,
    int numSets,
    double fuzziness
)
{
    if (featureIndex >= dataset.getNumFeatures())
    {
        throw std::out_of_range(
            "Feature index out of range."
        );
    }

    std::string variableName;

    if (!dataset.getFeatureNames().empty())
    {
        variableName =
            dataset.getFeatureName(featureIndex);
    }
    else
    {
        variableName =
            "Feature_" +
            std::to_string(featureIndex);
    }
    auto info =
        computeClusterInformation(
            dataset,
            featureIndex,
            numSets,
            fuzziness
        );
    FuzzyVariable variable(variableName,info.minimum,info.maximum);

    auto fuzzySets =
        generateGaussianSets(
            dataset,
            featureIndex,
            numSets,
            fuzziness
        );

    for (const auto& set : fuzzySets)
    {
        variable.addSet(*set);
    }

    return variable;
}
Vector FuzzySetGenerator::computeSigmas(
    const Vector& featureData,
    const std::vector<Cluster>& clusters,
    const Matrix& memberships,
    double fuzziness
)
{
    Vector sigmas(
        clusters.size(),
        0.0
    );

    for (size_t cluster = 0;
        cluster <clusters.size();
        cluster++)
    {
        double numerator = 0.0;

        double denominator = 0.0;

        for (size_t point = 0;
            point < featureData.size();
            point++)
        {
            double weight =
                std::pow(
                    memberships[point][clusters[cluster].originalIndex],fuzziness
                );

            double diff =
               featureData[point]
                -
                clusters[cluster].center;

            numerator +=
                weight *
                diff *
                diff;

            denominator +=
                weight;
        }

        sigmas[cluster] =
            std::sqrt(
                numerator /
                denominator
            );

        if (sigmas[cluster] <= 0.0)
        {
            sigmas[cluster] = 1e-6;
        }
    }

    return sigmas;
}
std::vector<std::shared_ptr<FuzzySet>>
FuzzySetGenerator::generateTriangleSets(
    const Dataset& dataset,
    size_t featureIndex,
    int numSets,
    double fuzziness
)
{
    auto info =
        computeClusterInformation(
            dataset,
            featureIndex,
            numSets,
            fuzziness
        );
    std::vector<std::shared_ptr<FuzzySet>> fuzzySets;

    fuzzySets.reserve(numSets);

    for (int i = 0; i < numSets; i++)
    {
        double left;
        double peak = info.clusters[i].center;
        double right;

        if (i == 0)
        {
            left = info.minimum;
        }
        else
        {
            left =
                (info.clusters[i - 1].center + info.clusters[i].center) / 2.0;
        }

        if (i == numSets - 1)
        {
            right = info.maximum;
        }
        else
        {
            right =
                (info.clusters[i].center + info.clusters[i + 1].center) / 2.0;
        }

        fuzzySets.push_back(
            std::make_shared<FuzzySet>(
               info.featureName + "_" + std::to_string(i + 1),

                std::make_shared<Triangle>(
                    info.featureName + "_" + std::to_string(i + 1),
                    left,
                    peak,
                    right
                    )
                )
        );
    }

    return fuzzySets;
}
FuzzyVariable
FuzzySetGenerator::generateTriangleVariable(
    const Dataset& dataset,
    size_t featureIndex,
    int numSets,
    double fuzziness
)
{
    if (featureIndex >= dataset.getNumFeatures())
    {
        throw std::out_of_range(
            "Feature index out of range."
        );
    }

    std::string variableName;

    if (!dataset.getFeatureNames().empty())
    {
        variableName =
            dataset.getFeatureName(featureIndex);
    }
    else
    {
        variableName =
            "Feature_" +
            std::to_string(featureIndex);
    }
    auto info =
        computeClusterInformation(
            dataset,
            featureIndex,
            numSets,
            fuzziness
        );
    FuzzyVariable variable(variableName,info.minimum,info.maximum);

    auto fuzzySets =
        generateTriangleSets(
            dataset,
            featureIndex,
            numSets,
            fuzziness
        );

    for (const auto& set : fuzzySets)
    {
        variable.addSet(*set);
    }

    return variable;
}
std::vector<std::shared_ptr<FuzzySet>>
FuzzySetGenerator::generateTrapezoidalSets(
    const Dataset& dataset,
    size_t featureIndex,
    int numSets,
    double fuzziness
)
{
    auto info =
        computeClusterInformation(
            dataset,
            featureIndex,
            numSets,
            fuzziness
        );
    std::vector<std::shared_ptr<FuzzySet>> fuzzySets;

    fuzzySets.reserve(numSets);

    for (int i = 0; i < numSets; i++)
    {
        double a;
        double b;
        double c;
        double d;

        if (i == 0)
        {
            double rightMid =
                (info.clusters[0].center + info.clusters[1].center) / 2.0;

            a = info.minimum;
            b = info.clusters[0].center;
            c = info.clusters[0].center;
            d = rightMid;
        }
        else if (i == numSets - 1)
        {
            double leftMid =
                (info.clusters[i - 1].center + info.clusters[i].center) / 2.0;
            a = leftMid;
            b = info.clusters[i].center;
            c = info.clusters[i].center;
            d = info.maximum;
        }
        else
        {
            double leftMid =
                (info.clusters[i - 1].center + info.clusters[i].center) / 2.0;

            double rightMid =
                (info.clusters[i].center + info.clusters[i + 1].center) / 2.0;

            double overlap =
                (rightMid - leftMid) / 4.0;

            a = leftMid;
            b = info.clusters[i].center - overlap;
            c = info.clusters[i].center + overlap;
            d = rightMid;
        }

        fuzzySets.push_back(
            std::make_shared<FuzzySet>(
                info.featureName +
                "_" +
                std::to_string(i + 1),

                std::make_shared<Trapezoidal>(
                    info.featureName + "_" + std::to_string(i + 1),
                    a,
                    b,
                    c,
                    d
                    )
                )
        );
    }

    return fuzzySets;
}
FuzzyVariable
FuzzySetGenerator::generateTrapezoidalVariable(
    const Dataset& dataset,
    size_t featureIndex,
    int numSets,
    double fuzziness
)
{
    if (featureIndex >= dataset.getNumFeatures())
    {
        throw std::out_of_range(
            "Feature index out of range."
        );
    }

    std::string variableName;

    if (!dataset.getFeatureNames().empty())
    {
        variableName =
            dataset.getFeatureName(featureIndex);
    }
    else
    {
        variableName =
            "Feature_" +
            std::to_string(featureIndex);
    }
    auto info =
        computeClusterInformation(
            dataset,
            featureIndex,
            numSets,
            fuzziness
        );
    FuzzyVariable variable(variableName,info.minimum,info.maximum);

    auto fuzzySets =
        generateTrapezoidalSets(
            dataset,
            featureIndex,
            numSets,
            fuzziness
        );

    for (const auto& set : fuzzySets)
    {
        variable.addSet(*set);
    }

    return variable;
}


