#include <iostream>
#include "../include/ML/Clustering/FuzzyCMeans.h"
#include <vector>

int main()
{
    // Dataset:
    // Feature 1 = Temperature
    // Feature 2 = Humidity

    Matrix data =
    {
        // Cool conditions
        {10, 20},
        {12, 25},
        {15, 30},

        // Hot conditions
        {80, 85},
        {85, 90},
        {90, 95}
    };


    // 2 clusters
    // fuzziness = 2
    // max iterations = 100
    // tolerance = 0.001

    FuzzyCMeans fcm(
        2,
        2.0,
        100,
        0.001
    );


    // Train model

    fcm.fit(data);



    // Display learned centers

    auto centers =
        fcm.getCenters();


    std::cout
        << "Cluster Centers:\n";


    for (int i = 0; i < centers.size(); i++)
    {
        std::cout
            << "Cluster "
            << i
            << ": ";


        for (double value : centers[i])
        {
            std::cout
                << value
                << " ";
        }


        std::cout << "\n";
    }



    // Predict unseen point

    Vector newPoint =
    {
        20,
        35
    };


    auto membership =
        fcm.predict(newPoint);
    


    std::cout
        << "\nPrediction for point (20,35):\n";


    for (int i = 0; i < membership.size(); i++)
    {
        std::cout
            << "Cluster "
            << i
            << " membership: "
            << membership[i]
            << "\n";
    }


    return 0;
}