#include <gtest/gtest.h>

#include "ML/Preprocessing/MinMaxScaler.h"

TEST(MinMaxScalerTest, FitComputesMinimumsCorrectly)
{
    Matrix data =
    {
        {1,5},
        {3,2},
        {0,10}
    };

    MinMaxScaler scaler;

    scaler.fit(data);

    Vector expected =
    {
        0,
        2
    };

    EXPECT_EQ(
        scaler.getMinimums(),
        expected
    );
}

TEST(MinMaxScalerTest, FitComputesMaximumsCorrectly)
{
    Matrix data =
    {
        {1,5},
        {3,2},
        {0,10}
    };

    MinMaxScaler scaler;

    scaler.fit(data);

    Vector expected =
    {
        3,
        10
    };

    EXPECT_EQ(
        scaler.getMaximums(),
        expected
    );
}

TEST(MinMaxScalerTest, TransformProducesValuesBetweenZeroAndOne)
{
    Matrix data =
    {
        {1},
        {3},
        {5}
    };

    MinMaxScaler scaler;

    Matrix scaled =
        scaler.fitTransform(data);

    for (const auto& row : scaled)
    {
        EXPECT_GE(row[0], 0.0);
        EXPECT_LE(row[0], 1.0);
    }
}

TEST(MinMaxScalerTest, TransformMatrixCorrectly)
{
    Matrix data =
    {
        {1},
        {3},
        {5}
    };

    MinMaxScaler scaler;

    scaler.fit(data);

    Matrix scaled =
        scaler.transform(data);

    EXPECT_DOUBLE_EQ(
        scaled[0][0],
        0.0
    );

    EXPECT_DOUBLE_EQ(
        scaled[1][0],
        0.5
    );

    EXPECT_DOUBLE_EQ(
        scaled[2][0],
        1.0
    );
}

TEST(MinMaxScalerTest, TransformSinglePoint)
{
    Matrix data =
    {
        {10},
        {20},
        {30}
    };

    MinMaxScaler scaler;

    scaler.fit(data);

    Vector point =
    {
        20
    };

    Vector result =
        scaler.transform(point);

    EXPECT_DOUBLE_EQ(
        result[0],
        0.5
    );
}

TEST(MinMaxScalerTest, ConstantFeatureTransformsToZero)
{
    Matrix data =
    {
        {5},
        {5},
        {5}
    };

    MinMaxScaler scaler;

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

TEST(MinMaxScalerTest, HandlesSingleFeature)
{
    Matrix data =
    {
        {10},
        {20},
        {30}
    };

    MinMaxScaler scaler;

    EXPECT_NO_THROW(
        scaler.fit(data)
    );
}

TEST(MinMaxScalerTest, HandlesMultipleFeatures)
{
    Matrix data =
    {
        {1,10,100},
        {2,20,200},
        {3,30,300}
    };

    MinMaxScaler scaler;

    EXPECT_NO_THROW(
        scaler.fit(data)
    );

    EXPECT_EQ(
        scaler.getMinimums().size(),
        3
    );

    EXPECT_EQ(
        scaler.getMaximums().size(),
        3
    );
}

TEST(MinMaxScalerTest, HandlesNegativeValues)
{
    Matrix data =
    {
        {-5},
        {0},
        {5}
    };

    MinMaxScaler scaler;

    Matrix scaled =
        scaler.fitTransform(data);

    EXPECT_DOUBLE_EQ(
        scaled[0][0],
        0.0
    );

    EXPECT_DOUBLE_EQ(
        scaled[1][0],
        0.5
    );

    EXPECT_DOUBLE_EQ(
        scaled[2][0],
        1.0
    );
}

TEST(MinMaxScalerTest, HandlesFloatingPointValues)
{
    Matrix data =
    {
        {1.5},
        {2.5},
        {3.5}
    };

    MinMaxScaler scaler;

    Matrix scaled =
        scaler.fitTransform(data);

    EXPECT_DOUBLE_EQ(
        scaled[0][0],
        0.0
    );

    EXPECT_DOUBLE_EQ(
        scaled[2][0],
        1.0
    );
}

TEST(MinMaxScalerTest, FitThrowsOnEmptyDataset)
{
    MinMaxScaler scaler;

    EXPECT_THROW(
        scaler.fit({}),
        std::invalid_argument
    );
}

TEST(MinMaxScalerTest, FitTransformEquivalentToFitThenTransform)
{
    Matrix data =
    {
        {1,2},
        {3,4},
        {5,6}
    };

    MinMaxScaler first;

    Matrix a =
        first.fitTransform(data);

    MinMaxScaler second;

    second.fit(data);

    Matrix b =
        second.transform(data);

    ASSERT_EQ(
        a.size(),
        b.size()
    );

    for (size_t i = 0; i < a.size(); i++)
    {
        for (size_t j = 0; j < a[i].size(); j++)
        {
            EXPECT_DOUBLE_EQ(
                a[i][j],
                b[i][j]
            );
        }
    }
}

TEST(MinMaxScalerTest, MinimumsRemainAvailableAfterTransform)
{
    Matrix data =
    {
        {1},
        {3},
        {5}
    };

    MinMaxScaler scaler;

    scaler.fit(data);

    scaler.transform(data);

    EXPECT_DOUBLE_EQ(
        scaler.getMinimums()[0],
        1.0
    );
}

TEST(MinMaxScalerTest, MaximumsRemainAvailableAfterTransform)
{
    Matrix data =
    {
        {1},
        {3},
        {5}
    };

    MinMaxScaler scaler;

    scaler.fit(data);

    scaler.transform(data);

    EXPECT_DOUBLE_EQ(
        scaler.getMaximums()[0],
        5.0
    );
}
TEST(MinMaxScalerTest, TransformBeforeFitThrows)
{
    MinMaxScaler scaler;

    Matrix data =
    {
        {1,2},
        {3,4}
    };

    EXPECT_THROW(
        scaler.transform(data),
        std::runtime_error
    );
}

TEST(MinMaxScalerTest, TransformVectorBeforeFitThrows)
{
    MinMaxScaler scaler;

    Vector point =
    {
        1,
        2
    };

    EXPECT_THROW(
        scaler.transform(point),
        std::runtime_error
    );
}

TEST(MinMaxScalerTest, TransformMatrixDimensionMismatchThrows)
{
    Matrix training =
    {
        {1,2},
        {3,4}
    };

    MinMaxScaler scaler;

    scaler.fit(training);

    Matrix wrongDimension =
    {
        {1,2,3},
        {4,5,6}
    };

    EXPECT_THROW(
        scaler.transform(wrongDimension),
        std::invalid_argument
    );
}

TEST(MinMaxScalerTest, TransformVectorDimensionMismatchThrows)
{
    Matrix training =
    {
        {1,2},
        {3,4}
    };

    MinMaxScaler scaler;

    scaler.fit(training);

    Vector wrongDimension =
    {
        1,
        2,
        3
    };

    EXPECT_THROW(
        scaler.transform(wrongDimension),
        std::invalid_argument
    );
}

TEST(MinMaxScalerTest, InconsistentRowSizesThrow)
{
    Matrix data =
    {
        {1,2},
        {3}
    };

    MinMaxScaler scaler;

    EXPECT_THROW(
        scaler.fit(data),
        std::invalid_argument
    );
}

TEST(MinMaxScalerTest, EmptyFeatureVectorThrows)
{
    Matrix data =
    {
        {},
        {}
    };

    MinMaxScaler scaler;

    EXPECT_THROW(
        scaler.fit(data),
        std::invalid_argument
    );
}

TEST(MinMaxScalerTest, EmptyMatrixTransformReturnsEmptyMatrix)
{
    Matrix training =
    {
        {1,2},
        {3,4}
    };

    MinMaxScaler scaler;

    scaler.fit(training);

    Matrix result =
        scaler.transform(Matrix{});

    EXPECT_TRUE(
        result.empty()
    );
}

TEST(MinMaxScalerTest, EmptyVectorDimensionMismatchThrows)
{
    Matrix training =
    {
        {1,2},
        {3,4}
    };

    MinMaxScaler scaler;

    scaler.fit(training);

    EXPECT_THROW(
        scaler.transform(Vector{}),
        std::invalid_argument
    );
}
TEST(MinMaxScalerTest, TransformDoesNotModifyOriginalMatrix)
{
    Matrix data =
    {
        {1},
        {2},
        {3}
    };

    Matrix original = data;

    MinMaxScaler scaler;

    scaler.fit(data);

    Matrix transformed =
        scaler.transform(data);

    EXPECT_EQ(
        data,
        original
    );

    EXPECT_NE(
        transformed,
        original
    );
}