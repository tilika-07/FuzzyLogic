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
    if (dataset.getNumSamples() == 0)
    {
        throw std::invalid_argument("Dataset is empty.");
    }

    if (dataset.getNumFeatures() == 0)
    {
        throw std::invalid_argument("Dataset has no features.");
    }

    if (featureIndex >= dataset.getNumFeatures())
    {
        throw std::out_of_range("Feature index out of range.");
    }

    if (numSets <= 0)
    {
        throw std::invalid_argument("Number of fuzzy sets must be positive.");
    }

    if (static_cast<size_t>(numSets) > dataset.getNumSamples())
    {
        throw std::invalid_argument(
            "Number of fuzzy sets cannot exceed number of samples.");
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
    Vector centers;
    centers.reserve(centerMatrix.size());

    for (const auto& c : centerMatrix)
    {
        centers.push_back(c[0]);
    }
    Vector sigmas = computeSigmas(
        info.featureData,
        info.clusters,
        info.memberships,
        fuzziness
    );
    for (auto& cluster : info.clusters)
    {
        cluster.sigma = sigmas[cluster.originalIndex];
    }
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

    Vector sigmas =
        computeSigmas(
            info.featureData,
            info.clusters,
            info.memberships,
            fuzziness
        );

    std::vector<std::shared_ptr<FuzzySet>> fuzzySets;
    fuzzySets.reserve(numSets);

    constexpr double overlapFactor = 0.25;
    if (numSets == 1)
    {
        double center = (info.minimum + info.maximum) / 2.0;

        fuzzySets.push_back(
            std::make_shared<FuzzySet>(
                info.featureName + "_1",
                std::make_shared<Triangle>(
                    info.featureName + "_1",
                    info.minimum,
                    center,
                    info.maximum
                    )
                )
        );

        return fuzzySets;
    }
    for (int i = 0; i < numSets; ++i)
    {
        double center = info.clusters[i].center;
        double sigma = sigmas[i];

        double left;
        double right;

        if (i == 0)
        {
            double rightMid =
                (center +
                    info.clusters[i + 1].center) / 2.0;

            left = info.minimum;

            right =
                std::min(
                    info.maximum,
                    rightMid + overlapFactor * sigma
                );
        }
        else if (i == numSets - 1)
        {
            double leftMid =
                (info.clusters[i - 1].center +
                    center) / 2.0;

            left =
                std::max(
                    info.minimum,
                    leftMid - overlapFactor * sigma
                );

            right = info.maximum;
        }
        else
        {
            double leftMid =
                (info.clusters[i - 1].center +
                    center) / 2.0;

            double rightMid =
                (center +
                    info.clusters[i + 1].center) / 2.0;

            left = leftMid - overlapFactor * sigma;
            right = rightMid + overlapFactor * sigma;
        }

        // Ensure a valid triangle.
        if (left >= center)
        {
            left = center - 1e-6;
        }

        if (right <= center)
        {
            right = center + 1e-6;
        }

        fuzzySets.push_back(
            std::make_shared<FuzzySet>(
                info.featureName + "_" +
                std::to_string(i + 1),

                std::make_shared<Triangle>(
                    info.featureName + "_" +
                    std::to_string(i + 1),
                    left,
                    center,
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

    Vector sigmas =
        computeSigmas(
            info.featureData,
            info.clusters,
            info.memberships,
            fuzziness
        );

    std::vector<std::shared_ptr<FuzzySet>> fuzzySets;
    fuzzySets.reserve(numSets);

    constexpr double supportFactor = 0.25;
    constexpr double plateauFactor = 0.5;
    if (numSets == 1)
    {
        fuzzySets.push_back(
            std::make_shared<FuzzySet>(
                info.featureName + "_1",
                std::make_shared<Trapezoidal>(
                    info.featureName + "_1",
                    info.minimum,
                    info.minimum,
                    info.maximum,
                    info.maximum
                    )
                )
        );

        return fuzzySets;
    }
    for (int i = 0; i < numSets; ++i)
    {
        double center = info.clusters[i].center;
        double sigma = sigmas[i];

        double a, b, c, d;

        if (i == 0)
        {
            double rightMid =
                (center +
                    info.clusters[i + 1].center) / 2.0;

            a = info.minimum;

            d = std::min(
                info.maximum,
                rightMid + supportFactor * sigma
            );
        }
        else if (i == numSets - 1)
        {
            double leftMid =
                (info.clusters[i - 1].center +
                    center) / 2.0;

            a = std::max(
                info.minimum,
                leftMid - supportFactor * sigma
            );

            d = info.maximum;
        }
        else
        {
            double leftMid =
                (info.clusters[i - 1].center +
                    center) / 2.0;

            double rightMid =
                (center +
                    info.clusters[i + 1].center) / 2.0;

            a = leftMid - supportFactor * sigma;
            d = rightMid + supportFactor * sigma;
        }

        b = std::max(a, center - plateauFactor * sigma);
        c = std::min(d, center + plateauFactor * sigma);

        if (b > c)
        {
            b = c = center;
        }

        fuzzySets.push_back(
            std::make_shared<FuzzySet>(
                info.featureName + "_" +
                std::to_string(i + 1),

                std::make_shared<Trapezoidal>(
                    info.featureName + "_" +
                    std::to_string(i + 1),
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


