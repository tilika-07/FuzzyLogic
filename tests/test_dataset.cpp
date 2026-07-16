#include <gtest/gtest.h>

#include "ML/Data/Dataset.h"

TEST(DatasetTest, ConstructorStoresData)
{
    Matrix data =
    {
        {1,2},
        {3,4}
    };

    Dataset dataset(data);

    EXPECT_EQ(
        dataset.getData(),
        data
    );
}

TEST(DatasetTest, ConstructorStoresFeatureNames)
{
    Dataset dataset(
        {
            {1,2},
            {3,4}
        },
        {
            "Temperature",
            "Humidity"
        }
        );

    EXPECT_EQ(
        dataset.getFeatureNames().size(),
        2
    );

    EXPECT_EQ(
        dataset.getFeatureName(0),
        "Temperature"
    );

    EXPECT_EQ(
        dataset.getFeatureName(1),
        "Humidity"
    );
}

TEST(DatasetTest, ConstructorStoresLabels)
{
    Dataset dataset(
        {
            {1,2},
            {3,4}
        },
        {
            "A",
            "B"
        },
        {
            "Cold",
            "Hot"
        }
        );

    EXPECT_EQ(
        dataset.getLabels().size(),
        2
    );

    EXPECT_EQ(
        dataset.getLabels()[0],
        "Cold"
    );

    EXPECT_EQ(
        dataset.getLabels()[1],
        "Hot"
    );
}

TEST(DatasetTest, SetDataUpdatesDataset)
{
    Dataset dataset;

    Matrix data =
    {
        {5,6},
        {7,8}
    };

    dataset.setData(data);

    EXPECT_EQ(
        dataset.getData(),
        data
    );
}

TEST(DatasetTest, SetFeatureNamesUpdatesNames)
{
    Dataset dataset;

    dataset.setFeatureNames(
        {
            "X",
            "Y"
        }
    );

    EXPECT_EQ(
        dataset.getFeatureNames().size(),
        2
    );

    EXPECT_EQ(
        dataset.getFeatureName(0),
        "X"
    );
}

TEST(DatasetTest, SetLabelsUpdatesLabels)
{
    Dataset dataset;

    dataset.setLabels(
        {
            "A",
            "B"
        }
    );

    EXPECT_EQ(
        dataset.getLabels().size(),
        2
    );

    EXPECT_EQ(
        dataset.getLabels()[1],
        "B"
    );
}

TEST(DatasetTest, ReturnsCorrectNumberOfSamples)
{
    Dataset dataset(
        {
            {1,2},
            {3,4},
            {5,6}
        }
    );

    EXPECT_EQ(
        dataset.getNumSamples(),
        3
    );
}

TEST(DatasetTest, ReturnsCorrectNumberOfFeatures)
{
    Dataset dataset(
        {
            {1,2,3},
            {4,5,6}
        }
    );

    EXPECT_EQ(
        dataset.getNumFeatures(),
        3
    );
}

TEST(DatasetTest, EmptyDatasetHasZeroDimensions)
{
    Dataset dataset;

    EXPECT_EQ(
        dataset.getNumSamples(),
        0
    );

    EXPECT_EQ(
        dataset.getNumFeatures(),
        0
    );
}

TEST(DatasetTest, GetFeatureByIndex)
{
    Dataset dataset(
        {
            {1,2},
            {3,4},
            {5,6}
        }
    );

    Vector feature =
        dataset.getFeature(1);

    Vector expected =
    {
        2,
        4,
        6
    };

    EXPECT_EQ(
        feature,
        expected
    );
}

TEST(DatasetTest, GetFeatureByName)
{
    Dataset dataset(
        {
            {1,2},
            {3,4}
        },
        {
            "X",
            "Y"
        }
        );

    Vector feature =
        dataset.getFeature("Y");

    Vector expected =
    {
        2,
        4
    };

    EXPECT_EQ(
        feature,
        expected
    );
}

TEST(DatasetTest, InvalidFeatureIndexThrows)
{
    Dataset dataset(
        {
            {1,2}
        }
    );

    EXPECT_THROW(
        dataset.getFeature(5),
        std::out_of_range
    );
}

TEST(DatasetTest, InvalidFeatureNameThrows)
{
    Dataset dataset(
        {
            {1,2}
        },
        {
            "A",
            "B"
        }
        );

    EXPECT_THROW(
        dataset.getFeature("Temperature"),
        std::invalid_argument
    );
}

TEST(DatasetTest, GetSample)
{
    Dataset dataset(
        {
            {1,2},
            {3,4}
        }
    );

    Vector expected =
    {
        3,
        4
    };

    EXPECT_EQ(
        dataset.getSample(1),
        expected
    );
}

TEST(DatasetTest, InvalidSampleIndexThrows)
{
    Dataset dataset(
        {
            {1,2}
        }
    );

    EXPECT_THROW(
        dataset.getSample(3),
        std::out_of_range
    );
}

TEST(DatasetTest, GetFeatureNameReturnsCorrectName)
{
    Dataset dataset(
        {
            {1,2}
        },
        {
            "Length",
            "Width"
        }
        );

    EXPECT_EQ(
        dataset.getFeatureName(0),
        "Length"
    );

    EXPECT_EQ(
        dataset.getFeatureName(1),
        "Width"
    );
}

TEST(DatasetTest, InvalidFeatureNameIndexThrows)
{
    Dataset dataset(
        {
            {1,2}
        },
        {
            "A",
            "B"
        }
        );

    EXPECT_THROW(
        dataset.getFeatureName(2),
        std::out_of_range
    );
}

TEST(DatasetTest, SupportsSingleSample)
{
    Dataset dataset(
        {
            {10,20,30}
        }
    );

    EXPECT_EQ(
        dataset.getNumSamples(),
        1
    );

    EXPECT_EQ(
        dataset.getNumFeatures(),
        3
    );
}

TEST(DatasetTest, SupportsSingleFeature)
{
    Dataset dataset(
        {
            {1},
            {2},
            {3}
        }
    );

    Vector expected =
    {
        1,
        2,
        3
    };

    EXPECT_EQ(
        dataset.getFeature(0),
        expected
    );
}

TEST(DatasetTest, SupportsNegativeValues)
{
    Dataset dataset(
        {
            {-1,-2},
            {-3,-4}
        }
    );

    EXPECT_DOUBLE_EQ(
        dataset.getData()[1][1],
        -4
    );
}

TEST(DatasetTest, SupportsFloatingPointValues)
{
    Dataset dataset(
        {
            {1.5,2.75},
            {3.125,4.875}
        }
    );

    EXPECT_DOUBLE_EQ(
        dataset.getData()[0][0],
        1.5
    );

    EXPECT_DOUBLE_EQ(
        dataset.getData()[1][1],
        4.875
    );
}