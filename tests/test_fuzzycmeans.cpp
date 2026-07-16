#include <gtest/gtest.h>
#include "../include/ML/Clustering/FuzzyCMeans.h"
#include <stdexcept>
#include <algorithm>
#include <cmath>
#include <vector>
#include <numeric>


TEST(FuzzyCMeansTest, ConstructorCreatesObject)
{
    FuzzyCMeans fcm(
        3,
        2.0,
        100,
        0.001
    );

    EXPECT_TRUE(
        fcm.getCenters().empty()
    );

    EXPECT_TRUE(
        fcm.getMemberships().empty()
    );
}


TEST(FuzzyCMeansTest, InvalidClusterCountThrows)
{
    EXPECT_THROW(
        FuzzyCMeans(0),
        std::invalid_argument
    );
}


TEST(FuzzyCMeansTest, InvalidFuzzinessThrows)
{
    EXPECT_THROW(
        FuzzyCMeans(
            3,
            1.0
        ),
        std::invalid_argument
    );
}


TEST(FuzzyCMeansTest, InvalidIterationsThrows)
{
    EXPECT_THROW(
        FuzzyCMeans(
            3,
            2.0,
            0
        ),
        std::invalid_argument
    );
}


TEST(FuzzyCMeansTest, InvalidToleranceThrows)
{
    EXPECT_THROW(
        FuzzyCMeans(
            3,
            2.0,
            100,
            -0.1
        ),
        std::invalid_argument
    );
}
TEST(FuzzyCMeansTest, FitCreatesClusterCenters)
{
    std::vector<std::vector<double>> data =
    {
        {1,1},
        {2,2},
        {100,100},
        {101,101}
    };


    FuzzyCMeans fcm(2);


    fcm.fit(data);


    auto centers =
        fcm.getCenters();


    EXPECT_EQ(
        centers.size(),
        2
    );


    EXPECT_EQ(
        centers[0].size(),
        2
    );
}
TEST(FuzzyCMeansTest, MembershipMatrixHasCorrectSize)
{
    std::vector<std::vector<double>> data =
    {
        {1,1},
        {2,2},
        {100,100},
        {101,101}
    };


    FuzzyCMeans fcm(2);


    fcm.fit(data);


    auto memberships =
        fcm.getMemberships();


    EXPECT_EQ(
        memberships.size(),
        data.size()
    );


    for (auto row : memberships)
    {
        EXPECT_EQ(
            row.size(),
            2
        );
    }
}
TEST(FuzzyCMeansTest, MembershipRowsSumToOne)
{
    std::vector<std::vector<double>> data =
    {
        {1,1},
        {2,2},
        {100,100},
        {101,101}
    };


    FuzzyCMeans fcm(2);


    fcm.fit(data);


    auto memberships =
        fcm.getMemberships();


    for (auto row : memberships)
    {
        double sum = 0;


        for (double value : row)
        {
            sum += value;
        }


        EXPECT_NEAR(
            sum,
            1.0,
            0.0001
        );
    }
}
TEST(FuzzyCMeansTest, EmptyDatasetThrows)
{
    FuzzyCMeans fcm(2);


    EXPECT_THROW(
        fcm.fit({}),
        std::invalid_argument
    );
}
TEST(FuzzyCMeansTest, DifferentDimensionsThrow)
{
    FuzzyCMeans fcm(2);


    std::vector<std::vector<double>> data =
    {
        {1,2},
        {3,4,5}
    };


    EXPECT_THROW(
        fcm.fit(data),
        std::invalid_argument
    );
}
TEST(FuzzyCMeansTest, PredictionSumsToOne)
{
    std::vector<std::vector<double>> data =
    {
        {1,1},
        {2,2},
        {100,100},
        {101,101}
    };


    FuzzyCMeans fcm(2);


    fcm.fit(data);


    auto result =
        fcm.predict({ 50,50 });


    double sum = 0;


    for (double value : result)
    {
        sum += value;
    }


    EXPECT_NEAR(
        sum,
        1.0,
        0.0001
    );
}
TEST(FuzzyCMeansTest, PredictBeforeFitThrows)
{
    FuzzyCMeans fcm(2);


    EXPECT_THROW(
        fcm.predict({ 1,1 }),
        std::runtime_error
    );
}
TEST(FuzzyCMeansTest, LearnsCorrectClusterCenters)
{
    std::vector<std::vector<double>> data =
    {
        {1,1},
        {2,2},
        {100,100},
        {101,101}
    };


    FuzzyCMeans fcm(2);

    fcm.fit(data);


    auto centers =
        fcm.getCenters();


    bool foundSmallCluster = false;
    bool foundLargeCluster = false;


    for (auto c : centers)
    {
        if (c[0] < 10)
        {
            foundSmallCluster = true;
        }

        if (c[0] > 90)
        {
            foundLargeCluster = true;
        }
    }


    EXPECT_TRUE(foundSmallCluster);

    EXPECT_TRUE(foundLargeCluster);
}
TEST(FuzzyCMeansTest, PredictsHigherMembershipForCloserCluster)
{
    std::vector<std::vector<double>> data =
    {
        {0,0},
        {1,1},
        {50,50},
        {51,51}
    };


    FuzzyCMeans fcm(2);

    fcm.fit(data);


    auto prediction =
        fcm.predict({ 0.5,0.5 });


    EXPECT_GT(
        std::max(
            prediction[0],
            prediction[1]
        ),
        0.8
    );
}
TEST(FuzzyCMeansTest, ExactCenterGetsFullMembership)
{
    std::vector<std::vector<double>> data =
    {
        {10,10},
        {100,100}
    };


    FuzzyCMeans fcm(2);

    fcm.fit(data);


    auto centers =
        fcm.getCenters();


    auto result =
        fcm.predict(
            centers[0]
        );


    EXPECT_NEAR(
        result[0],
        1.0,
        0.001
    );
}
TEST(FuzzyCMeansTest, SupportsOneDimensionalData)
{
    std::vector<std::vector<double>> data =
    {
        {1},
        {2},
        {100},
        {101}
    };


    FuzzyCMeans fcm(2);


    EXPECT_NO_THROW(
        fcm.fit(data)
    );
}
TEST(FuzzyCMeansTest, SupportsHighDimensionalData)
{
    std::vector<std::vector<double>> data =
    {
        {1,2,3,4},
        {2,3,4,5},
        {100,101,102,103}
    };


    FuzzyCMeans fcm(2);


    fcm.fit(data);


    EXPECT_EQ(
        fcm.getCenters()[0].size(),
        4
    );
}
TEST(FuzzyCMeansTest, AllowsMultipleFits)
{
    FuzzyCMeans fcm(2);


    EXPECT_NO_THROW(
        fcm.fit(
            {
                {1,1},
                {2,2}
            }
        )
    );


    EXPECT_NO_THROW(
        fcm.fit(
            {
                {50,50},
                {60,60}
            }
        )
    );
}
TEST(FuzzyCMeansTest, MoreClustersThanPointsThrows)
{
    FuzzyCMeans fcm(5);

    EXPECT_THROW(
        fcm.fit(
            {
                {1,1},
                {2,2}
            }
        ),
        std::invalid_argument
    );
}
TEST(FuzzyCMeansTest, MembershipValuesAreBetweenZeroAndOne)
{
    FuzzyCMeans fcm(2);

    fcm.fit(
        {
            {1,1},
            {2,2},
            {100,100},
            {101,101}
        }
    );


    auto memberships =
        fcm.getMemberships();


    for (auto row : memberships)
    {
        for (double value : row)
        {
            EXPECT_GE(value, 0.0);

            EXPECT_LE(value, 1.0);
        }
    }
}
TEST(FuzzyCMeansTest, PredictionValuesAreBetweenZeroAndOne)
{
    FuzzyCMeans fcm(2);

    fcm.fit(
        {
            {1,1},
            {100,100}
        }
    );


    auto result =
        fcm.predict({ 20,20 });


    for (double value : result)
    {
        EXPECT_GE(value, 0.0);

        EXPECT_LE(value, 1.0);
    }
}
TEST(FuzzyCMeansTest, SingleClusterAlwaysFullMembership)
{
    FuzzyCMeans fcm(1);


    fcm.fit(
        {
            {1,1},
            {2,2},
            {3,3}
        }
    );


    auto result =
        fcm.predict({ 5,5 });


    EXPECT_NEAR(
        result[0],
        1.0,
        0.0001
    );
}
TEST(FuzzyCMeansTest, HandlesDuplicatePoints)
{
    FuzzyCMeans fcm(2);


    EXPECT_NO_THROW(
        fcm.fit(
            {
                {1,1},
                {1,1},
                {100,100},
                {100,100}
            }
        )
    );
}

TEST(FuzzyCMeansTest, DoesNotProduceNaN)
{
    FuzzyCMeans fcm(2);


    fcm.fit(
        {
            {1,1},
            {2,2},
            {100,100}
        }
    );


    auto centers =
        fcm.getCenters();


    for (auto center : centers)
    {
        for (double value : center)
        {
            EXPECT_FALSE(
                std::isnan(value)
            );
        }
    }
}
TEST(FuzzyCMeansTest, AmbiguousPointGetsFuzzyMembership)
{
    std::vector<std::vector<double>> data =
    {
        {0,0},
        {1,1},

        {10,10},
        {11,11}
    };


    FuzzyCMeans fcm(
        2,
        2.0
    );


    fcm.fit(data);


    auto result =
        fcm.predict(
            { 5,5 }
    );


    EXPECT_GT(
        result[0],
        0.2
    );


    EXPECT_GT(
        result[1],
        0.2
    );
}
TEST(FuzzyCMeansTest, HandlesOverlappingClusters)
{
    std::vector<std::vector<double>> data =
    {
        {1,1},
        {2,2},
        {3,3},

        {5,5},

        {7,7},
        {8,8},
        {9,9}
    };


    FuzzyCMeans fcm(2);


    EXPECT_NO_THROW(
        fcm.fit(data)
    );


    auto prediction =
        fcm.predict({ 5,5 });


    EXPECT_GT(
        prediction[0],
        0.1
    );


    EXPECT_GT(
        prediction[1],
        0.1
    );
}
TEST(FuzzyCMeansTest, HigherFuzzinessProducesSofterMemberships)
{
    std::vector<std::vector<double>> data =
    {
        {0,0},
        {10,10}
    };


    FuzzyCMeans lowFuzzy(
        2,
        1.5
    );


    FuzzyCMeans highFuzzy(
        2,
        5.0
    );


    lowFuzzy.fit(data);

    highFuzzy.fit(data);


    auto low =
        lowFuzzy.predict({ 3,3 });


    auto high =
        highFuzzy.predict({ 3,3 });


    double lowDifference =
        std::abs(
            low[0] - low[1]
        );


    double highDifference =
        std::abs(
            high[0] - high[1]
        );


    EXPECT_LT(
        highDifference,
        lowDifference
    );
}

TEST(FuzzyCMeansTest, HandlesDifferentFeatureScales)
{
    FuzzyCMeans fcm(2);

    EXPECT_NO_THROW(
        fcm.fit(
            {
                {1,1000},
                {2,1200},
                {100,90000},
                {101,91000}
            }
        )
    );
}
TEST(FuzzyCMeansTest, EmptyPointsThrow)
{
    FuzzyCMeans fcm(2);


    EXPECT_THROW(
        fcm.fit(
            {
                {},
                {}
            }
        ),
        std::invalid_argument
    );
}
TEST(FuzzyCMeansTest, HandlesLargeDataset)
{
    std::vector<std::vector<double>> data;


    for (int i = 0; i < 1000; i++)
    {
        data.push_back(
            { double(i), double(i) }
        );
    }


    FuzzyCMeans fcm(3);


    EXPECT_NO_THROW(
        fcm.fit(data)
    );
}