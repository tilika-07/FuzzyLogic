#include <gtest/gtest.h>

#include "ML/Preprocessing/StandardScaler.h"

#include <cmath>
#include <numeric>

TEST(StandardScalerTest, FitComputesMeansCorrectly)
{
    Matrix data =
    {
        {1,2},
        {3,4},
        {5,6}
    };

    StandardScaler scaler;

    scaler.fit(data);

    Vector expected =
    {
        3.0,
        4.0
    };

    EXPECT_EQ(
        scaler.getMeans(),
        expected
    );
}

TEST(StandardScalerTest, FitComputesStandardDeviationsCorrectly)
{
    Matrix data =
    {
        {1},
        {3},
        {5}
    };

    StandardScaler scaler;

    scaler.fit(data);

    EXPECT_NEAR(
        scaler.getStdDevs()[0],
        std::sqrt(8.0 / 3.0),
        1e-6
    );
}

TEST(StandardScalerTest, FitTransformProducesZeroMean)
{
    Matrix data =
    {
        {1,2},
        {3,4},
        {5,6}
    };

    StandardScaler scaler;

    Matrix scaled =
        scaler.fitTransform(data);

    for (size_t feature = 0;
        feature < scaled[0].size();
        feature++)
    {
        double mean = 0.0;

        for (const auto& row : scaled)
        {
            mean += row[feature];
        }

        mean /= scaled.size();

        EXPECT_NEAR(
            mean,
            0.0,
            1e-6
        );
    }
}

TEST(StandardScalerTest, FitTransformProducesUnitVariance)
{
    Matrix data =
    {
        {1},
        {3},
        {5}
    };

    StandardScaler scaler;

    Matrix scaled =
        scaler.fitTransform(data);

    double variance = 0.0;

    for (const auto& row : scaled)
    {
        variance += row[0] * row[0];
    }

    variance /= scaled.size();

    EXPECT_NEAR(
        variance,
        1.0,
        1e-6
    );
}

TEST(StandardScalerTest, TransformMatrix)
{
    Matrix data =
    {
        {1},
        {3},
        {5}
    };

    StandardScaler scaler;

    scaler.fit(data);

    Matrix transformed =
        scaler.transform(data);

    EXPECT_NEAR(
        transformed[0][0],
        -1.224744871,
        1e-6
    );

    EXPECT_NEAR(
        transformed[1][0],
        0.0,
        1e-6
    );

    EXPECT_NEAR(
        transformed[2][0],
        1.224744871,
        1e-6
    );
}

TEST(StandardScalerTest, TransformSinglePoint)
{
    Matrix data =
    {
        {1},
        {3},
        {5}
    };

    StandardScaler scaler;

    scaler.fit(data);

    Vector point =
    {
        3
    };

    Vector result =
        scaler.transform(point);

    EXPECT_NEAR(
        result[0],
        0.0,
        1e-6
    );
}

TEST(StandardScalerTest, ConstantFeatureGetsZeroAfterScaling)
{
    Matrix data =
    {
        {5},
        {5},
        {5}
    };

    StandardScaler scaler;

    Matrix scaled =
        scaler.fitTransform(data);

    for (const auto& row : scaled)
    {
        EXPECT_DOUBLE_EQ(
            row[0],
            0.0
        );
    }
}

TEST(StandardScalerTest, HandlesSingleFeature)
{
    Matrix data =
    {
        {10},
        {20},
        {30}
    };

    StandardScaler scaler;

    EXPECT_NO_THROW(
        scaler.fit(data)
    );
}

TEST(StandardScalerTest, HandlesMultipleFeatures)
{
    Matrix data =
    {
        {1,10,100},
        {2,20,200},
        {3,30,300}
    };

    StandardScaler scaler;

    EXPECT_NO_THROW(
        scaler.fit(data)
    );

    EXPECT_EQ(
        scaler.getMeans().size(),
        3
    );
}

TEST(StandardScalerTest, HandlesNegativeValues)
{
    Matrix data =
    {
        {-3},
        {-1},
        {1},
        {3}
    };

    StandardScaler scaler;

    Matrix scaled =
        scaler.fitTransform(data);

    double mean = 0.0;

    for (const auto& row : scaled)
    {
        mean += row[0];
    }

    mean /= scaled.size();

    EXPECT_NEAR(
        mean,
        0.0,
        1e-6
    );
}

TEST(StandardScalerTest, HandlesFloatingPointValues)
{
    Matrix data =
    {
        {1.5},
        {2.5},
        {3.5}
    };

    StandardScaler scaler;

    EXPECT_NO_THROW(
        scaler.fit(data)
    );
}

TEST(StandardScalerTest, FitThrowsOnEmptyDataset)
{
    StandardScaler scaler;

    EXPECT_THROW(
        scaler.fit({}),
        std::invalid_argument
    );
}

TEST(StandardScalerTest, FitTransformEquivalentToFitThenTransform)
{
    Matrix data =
    {
        {1,2},
        {3,4},
        {5,6}
    };

    StandardScaler first;

    Matrix a =
        first.fitTransform(data);

    StandardScaler second;

    second.fit(data);

    Matrix b =
        second.transform(data);

    EXPECT_EQ(
        a.size(),
        b.size()
    );

    for (size_t i = 0; i < a.size(); i++)
    {
        for (size_t j = 0; j < a[i].size(); j++)
        {
            EXPECT_NEAR(
                a[i][j],
                b[i][j],
                1e-9
            );
        }
    }
}

TEST(StandardScalerTest, MeansRemainAvailableAfterTransform)
{
    Matrix data =
    {
        {1},
        {3},
        {5}
    };

    StandardScaler scaler;

    scaler.fit(data);

    scaler.transform(data);

    EXPECT_DOUBLE_EQ(
        scaler.getMeans()[0],
        3.0
    );
}

TEST(StandardScalerTest, StandardDeviationsRemainAvailableAfterTransform)
{
    Matrix data =
    {
        {1},
        {3},
        {5}
    };

    StandardScaler scaler;

    scaler.fit(data);

    scaler.transform(data);

    EXPECT_NEAR(
        scaler.getStdDevs()[0],
        std::sqrt(8.0 / 3.0),
        1e-6
    );
}
