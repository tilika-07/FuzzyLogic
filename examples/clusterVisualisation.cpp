#include <iostream>
#include <vector>
#include "../include/FuzzyLogic.h"
#include "visualization/JsonExporter.h"

using namespace fuzzy;

int main()
{
    Matrix dataset =
    {
        {10.0, 20.0},
        {12.0, 18.0},
        {15.0, 25.0},
        {18.0, 22.0},

        {70.0, 80.0},
        {72.0, 84.0},
        {75.0, 78.0},
        {80.0, 85.0},

        {40.0, 55.0},
        {42.0, 58.0},
        {45.0, 52.0},
        {48.0, 60.0}
    };

    constexpr int clusters = 3;
    constexpr double fuzziness = 2.0;
    constexpr int maxIterations = 100;
    constexpr double tolerance = 1e-4;

    FuzzyCMeans fcm(
        clusters,
        fuzziness,
        maxIterations,
        tolerance
    );

    fcm.fit(dataset);

    Vector queryPoint =
    {
        20.0,
        30.0
    };

    std::vector<std::string> featureNames =
    {
        "Feature 1",
        "Feature 2"
    };

    fuzzy::visualization::JsonExporter::exportFCMClusters(
        fcm,
        dataset,
        queryPoint,
        featureNames,
        "examples/output/clusters.json"
    );
  
    std::cout
        << "Cluster visualization exported to:\n"
        << "examples/output/clusters.json\n";

    return 0;
}