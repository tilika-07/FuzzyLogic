#include <nlohmann/json.hpp>
#include "visualization/JsonExporter.h"
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <filesystem>
#include <stdexcept>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace
{

    json createMembershipJson(
        const FuzzyVariable& variable,
        double step)
    {
        if (step <= 0.0)
        {
            throw std::invalid_argument(
                "Sampling step must be greater than zero.");
        }

        json root;

        root["type"] = "membership";

        root["variable"] =
        {
            { "name", variable.getName() },
            { "min", variable.getMinValue() },
            { "max", variable.getMaxValue() }
        };

        root["samplingStep"] = step;

        root["sets"] = json::array();

        const auto& sets = variable.getSets();

        for (const auto& set : sets)
        {
            json setJson;

            setJson["name"] = set.getName();

            // Metadata only.
            // Plotting uses sampled points.
            if (set.getMembershipFunction() != nullptr)
            {
                setJson["membershipFunction"] =
                    set.getMembershipFunction()->getName();
            }

            setJson["points"] = json::array();

            for (double x = variable.getMinValue();
                x <= variable.getMaxValue();
                x += step)
            {
                setJson["points"].push_back(
                    {
                        { "x", x },
                        { "membership", set.membership(x) }
                    });
            }

            // Ensure the exact maximum value is sampled
            if (variable.getMaxValue() >
                variable.getMinValue())
            {
                double maxX = variable.getMaxValue();

                if (setJson["points"].empty() ||
                    setJson["points"].back()["x"] != maxX)
                {
                    setJson["points"].push_back(
                        {
                            { "x", maxX },
                            { "membership", set.membership(maxX) }
                        });
                }
            }

            root["sets"].push_back(std::move(setJson));
        }

        return root;
    }

    void writeJson(
        const json& root,
        const std::string& filename)
    {
        std::filesystem::path path(filename);

        if (!path.parent_path().empty())
        {
            std::filesystem::create_directories(path.parent_path());
        }
        std::cout << "Current working directory: "
            << std::filesystem::current_path() << '\n';

        std::cout << "Writing to: "
            << std::filesystem::absolute(path) << '\n';
        std::ofstream out(filename);

        if (!out)
        {
            throw std::runtime_error(
                "Unable to open output file: " +
                filename);
        }

        out << std::setw(4) << root;
    }

} 



namespace fuzzy
{
    namespace visualization
    {

        void JsonExporter::exportMembershipFunctions(
            const FuzzyVariable& variable,
            const std::string& filename,
            double step)
        {
            json root =
                createMembershipJson(
                    variable,
                    step);

            writeJson(root, filename);
        }
        json createFCMJson(
            const FuzzyCMeans& fcm,
            const Matrix& dataset,
            const Vector& queryPoint,
            const std::vector<std::string>& featureNames)
        {
            if (dataset.empty())
            {
                throw std::invalid_argument(
                    "Dataset cannot be empty.");
            }

            const std::size_t dimensions = dataset.front().size();

            if (dimensions == 0)
            {
                throw std::invalid_argument(
                    "Dataset must contain at least one feature.");
            }

            for (const auto& point : dataset)
            {
                if (point.size() != dimensions)
                {
                    throw std::invalid_argument(
                        "All dataset points must have the same dimensionality.");
                }
            }

            if (queryPoint.size() != dimensions)
            {
                throw std::invalid_argument(
                    "Query point dimensionality does not match dataset.");
            }

            if (featureNames.size() != dimensions)
            {
                throw std::invalid_argument(
                    "Number of feature names does not match dataset dimensionality.");
            }

            Matrix centers = fcm.getCenters();
            Matrix memberships = fcm.getMemberships();

            if (memberships.size() != dataset.size())
            {
                throw std::runtime_error(
                    "Membership matrix size does not match dataset.");
            }

            json root;

            root["type"] = "fcm";

            root["featureNames"] = featureNames;

            root["dataset"] = json::array();

            for (std::size_t i = 0; i < dataset.size(); ++i)
            {
                json pointJson;

                pointJson["coordinates"] = dataset[i];
                pointJson["memberships"] = memberships[i];

                auto maxIt = std::max_element(
                    memberships[i].begin(),
                    memberships[i].end());

                pointJson["dominantCluster"] =
                    static_cast<int>(
                        std::distance(
                            memberships[i].begin(),
                            maxIt));

                root["dataset"].push_back(std::move(pointJson));
            }

            root["centers"] = centers;

            Vector predictedMemberships =
                fcm.predict(queryPoint);

            json query;

            query["coordinates"] = queryPoint;
            query["memberships"] = predictedMemberships;
            query["predictedCluster"] =
                fcm.predictCluster(queryPoint);

            root["queryPoint"] = std::move(query);

            return root;
        }



        void JsonExporter::exportFCMClusters(
            const FuzzyCMeans& fcm,
            const Matrix& dataset,
            const Vector& queryPoint,
            const std::vector<std::string>& featureNames,
            const std::string& filename)
        {
            json root =
                createFCMJson(
                    fcm,
                    dataset,
                    queryPoint,
                    featureNames);

            writeJson(root, filename);
        }
    }
}