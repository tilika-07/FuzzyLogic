#include <gtest/gtest.h>

#include "ML/Data/CSVWriter.h"
#include "ML/Data/CSVReader.h"
#include "ML/Data/Dataset.h"

#include <cstdio>

TEST(CSVWriterTest, WritesValidDataset)
{
    Dataset dataset(
        {
            {1,2},
            {3,4}
        },
        {
            "A",
            "B"
        }
        );

    const std::string file = "writer_valid.csv";

    CSVWriter::write(file, dataset);

    Dataset loaded =
        CSVReader::read(file);

    EXPECT_EQ(
        loaded.getNumSamples(),
        2
    );

    EXPECT_EQ(
        loaded.getNumFeatures(),
        2
    );

    EXPECT_EQ(
        loaded.getFeatureNames(),
        dataset.getFeatureNames()
    );

    EXPECT_EQ(
        loaded.getData(),
        dataset.getData()
    );

    std::remove(file.c_str());
}

TEST(CSVWriterTest, WritesDatasetWithoutHeaders)
{
    Dataset dataset(
        {
            {5,6},
            {7,8}
        }
    );

    const std::string file =
        "no_headers.csv";

    CSVWriter::write(file, dataset);

    Dataset loaded =
        CSVReader::read(
            file,
            false
        );

    EXPECT_EQ(
        loaded.getData(),
        dataset.getData()
    );

    EXPECT_TRUE(
        loaded.getFeatureNames().empty()
    );

    std::remove(file.c_str());
}

TEST(CSVWriterTest, EmptyDatasetThrows)
{
    Dataset dataset;

    EXPECT_THROW(
        CSVWriter::write(
            "empty.csv",
            dataset
        ),
        std::invalid_argument
    );
}

TEST(CSVWriterTest, EmptyFeatureVectorThrows)
{
    Dataset dataset(
        {
            {},
            {}
        }
    );

    EXPECT_THROW(
        CSVWriter::write(
            "empty_features.csv",
            dataset
        ),
        std::invalid_argument
    );
}

TEST(CSVWriterTest, HeaderCountMismatchThrows)
{
    Dataset dataset(
        {
            {1,2},
            {3,4}
        },
        {
            "A",
            "B",
            "C"
        }
        );

    EXPECT_THROW(
        CSVWriter::write(
            "bad_header.csv",
            dataset
        ),
        std::invalid_argument
    );
}

TEST(CSVWriterTest, InconsistentRowSizesThrow)
{
    Dataset dataset(
        {
            {1,2},
            {3}
        },
        {
            "A",
            "B"
        }
        );

    EXPECT_THROW(
        CSVWriter::write(
            "bad_rows.csv",
            dataset
        ),
        std::invalid_argument
    );
}

TEST(CSVWriterTest, SupportsCustomDelimiter)
{
    Dataset dataset(
        {
            {1,2},
            {3,4}
        },
        {
            "A",
            "B"
        }
        );

    const std::string file =
        "semicolon.csv";

    CSVWriter::write(
        file,
        dataset,
        ';'
    );

    Dataset loaded =
        CSVReader::read(
            file,
            true,
            ';'
        );

    EXPECT_EQ(
        loaded.getData(),
        dataset.getData()
    );

    EXPECT_EQ(
        loaded.getFeatureNames(),
        dataset.getFeatureNames()
    );

    std::remove(file.c_str());
}

TEST(CSVWriterTest, OverwritesExistingFile)
{
    Dataset first(
        {
            {1}
        },
        {
            "A"
        }
        );

    Dataset second(
        {
            {100}
        },
        {
            "A"
        }
        );

    const std::string file =
        "overwrite.csv";

    CSVWriter::write(
        file,
        first
    );

    CSVWriter::write(
        file,
        second
    );

    Dataset loaded =
        CSVReader::read(file);

    EXPECT_EQ(
        loaded.getNumSamples(),
        1
    );

    EXPECT_DOUBLE_EQ(
        loaded.getData()[0][0],
        100
    );

    std::remove(file.c_str());
}

TEST(CSVWriterTest, PreservesFloatingPointValues)
{
    Dataset dataset(
        {
            {1.23456789, 9.87654321}
        },
        {
            "X",
            "Y"
        }
        );

    const std::string file =
        "floating.csv";

    CSVWriter::write(
        file,
        dataset
    );

    Dataset loaded =
        CSVReader::read(file);

    EXPECT_NEAR(
        loaded.getData()[0][0],
        1.23456789,
        1e-9
    );

    EXPECT_NEAR(
        loaded.getData()[0][1],
        9.87654321,
        1e-9
    );

    std::remove(file.c_str());
}

TEST(CSVWriterTest, WritesSingleRowDataset)
{
    Dataset dataset(
        {
            {10,20,30}
        },
        {
            "A",
            "B",
            "C"
        }
        );

    const std::string file =
        "single_row.csv";

    CSVWriter::write(
        file,
        dataset
    );

    Dataset loaded =
        CSVReader::read(file);

    EXPECT_EQ(
        loaded.getNumSamples(),
        1
    );

    EXPECT_EQ(
        loaded.getData(),
        dataset.getData()
    );

    std::remove(file.c_str());
}

TEST(CSVWriterTest, WritesSingleColumnDataset)
{
    Dataset dataset(
        {
            {10},
            {20},
            {30}
        },
        {
            "Temperature"
        }
        );

    const std::string file =
        "single_column.csv";

    CSVWriter::write(
        file,
        dataset
    );

    Dataset loaded =
        CSVReader::read(file);

    EXPECT_EQ(
        loaded.getNumFeatures(),
        1
    );

    EXPECT_EQ(
        loaded.getData(),
        dataset.getData()
    );

    std::remove(file.c_str());
}

TEST(CSVWriterTest, WritesLargeDataset)
{
    Matrix data;

    for (int i = 0; i < 1000; i++)
    {
        data.push_back(
            {
                static_cast<double>(i),
                static_cast<double>(i * 2)
            }
        );
    }

    Dataset dataset(
        data,
        {
            "A",
            "B"
        }
    );

    const std::string file =
        "large.csv";

    CSVWriter::write(
        file,
        dataset
    );

    Dataset loaded =
        CSVReader::read(file);

    EXPECT_EQ(
        loaded.getNumSamples(),
        1000
    );

    EXPECT_EQ(
        loaded.getData(),
        dataset.getData()
    );

    std::remove(file.c_str());
}