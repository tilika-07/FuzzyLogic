#pragma once
#include <memory>
#include <vector>
#include <stdexcept>
#include "ML/Data/Dataset.h"
#include "FuzzySet.h"
#include "FuzzyVariable.h"
class FuzzySetGenerator
{
public:
    static std::vector<std::shared_ptr<FuzzySet>>
        generateGaussianSets(
            const Dataset& dataset,
            size_t featureIndex,
            int numSets,
            double fuzziness = 2.0
        );
    static FuzzyVariable generateGaussianVariable(
        const Dataset& dataset,
        size_t featureIndex,
        int numSets,
        double fuzziness = 2.0
    );
    static std::vector<std::shared_ptr<FuzzySet>>
        generateTriangleSets(
            const Dataset& dataset,
            size_t featureIndex,
            int numSets,
            double fuzziness = 2.0
        );

    static FuzzyVariable
        generateTriangleVariable(
            const Dataset& dataset,
            size_t featureIndex,
            int numSets,
            double fuzziness = 2.0
        );
    static std::vector<std::shared_ptr<FuzzySet>>
        generateTrapezoidalSets(
            const Dataset& dataset,
            size_t featureIndex,
            int numSets,
            double fuzziness = 2.0
        );

    static FuzzyVariable
        generateTrapezoidalVariable(
            const Dataset& dataset,
            size_t featureIndex,
            int numSets,
            double fuzziness = 2.0
        );


private:
    struct Cluster
    {
        double center;
        size_t originalIndex;
        double sigma;
    };

    struct ClusterInformation
    {
        Vector featureData;
        std::vector<Cluster> clusters;
        Matrix memberships;
        std::string featureName;
        double minimum=0.0;
        double maximum=0.0;
    };

    static ClusterInformation computeClusterInformation(
        const Dataset& dataset,
        size_t featureIndex,
        int numSets,
        double fuzziness
    );
 

    static Vector computeSigmas(
        const Vector& featureData,
        const std::vector<Cluster>& clusters,
        const Matrix& memberships,
        double fuzziness
    );
};