#ifndef JSONEXPORTER_H
#define JSONEXPORTER_H
#include <string>
#include <vector>
#include "FuzzyVariable.h"
#include "ML/Clustering/FuzzyCMeans.h"

namespace fuzzy
{
    namespace visualization
    {

        class JsonExporter
        {
        public:

            static void exportMembershipFunctions(
                const FuzzyVariable& variable,
                const std::string& filename,
                double step = 0.1
            );

            
            static void exportFCMClusters(
                const FuzzyCMeans& fcm,
                const Matrix& dataset,
                const Vector& queryPoint,
                const std::vector<std::string>& featureNames,
                const std::string& filename
            );
        };

    } // namespace visualization
} // namespace fuzzy

#endif