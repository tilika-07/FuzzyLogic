#include <gtest/gtest.h>

#include "ML/Data/CSVReader.h"

#include <fstream>
#include <cstdio>

TEST(CSVReaderTest, ReadsValidCSV)
{
    const std::string file = "reader_valid.csv";

    std::ofstream out(file);

    out << "A,B,C\n";
    out << "1,2,3\n";
    out << "4,5,6\n";

    out.close();

    Dataset dataset =
        CSVReader::read(file);

    EXPECT_EQ(dataset.getNumSamples(), 2);
    EXPECT_EQ(dataset.getNumFeatures(), 3);

    EXPECT_EQ(dataset.getFeatureNames()[0], "A");
    EXPECT_EQ(dataset.getFeatureNames()[1], "B");
    EXPECT_EQ(dataset.getFeatureNames()[2], "C");

    EXPECT_DOUBLE_EQ(dataset.getData()[0][0], 1);
    EXPECT_DOUBLE_EQ(dataset.getData()[0][1], 2);
    EXPECT_DOUBLE_EQ(dataset.getData()[0][2], 3);

    EXPECT_DOUBLE_EQ(dataset.getData()[1][0], 4);
    EXPECT_DOUBLE_EQ(dataset.getData()[1][1], 5);
    EXPECT_DOUBLE_EQ(dataset.getData()[1][2], 6);

    std::remove(file.c_str());
}

TEST(CSVReaderTest, ReadsWithoutHeader)
{
    const std::string file = "reader_no_header.csv";

    std::ofstream out(file);

    out << "1,2\n";
    out << "3,4\n";

    out.close();

    Dataset dataset =
        CSVReader::read(
            file,
            false
        );

    EXPECT_EQ(dataset.getNumSamples(), 2);
    EXPECT_EQ(dataset.getNumFeatures(), 2);

    EXPECT_TRUE(
        dataset.getFeatureNames().empty()
    );

    EXPECT_DOUBLE_EQ(dataset.getData()[1][0], 3);
    EXPECT_DOUBLE_EQ(dataset.getData()[1][1], 4);

    std::remove(file.c_str());
}

TEST(CSVReaderTest, MissingFileThrows)
{
    EXPECT_THROW(
        CSVReader::read("missing.csv"),
        std::runtime_error
    );
}

TEST(CSVReaderTest, EmptyFileThrows)
{
    const std::string file = "empty.csv";

    std::ofstream(file).close();

    EXPECT_THROW(
        CSVReader::read(file),
        std::runtime_error
    );

    std::remove(file.c_str());
}

TEST(CSVReaderTest, HeaderOnlyThrows)
{
    const std::string file = "header_only.csv";

    std::ofstream out(file);

    out << "A,B,C\n";

    out.close();

    EXPECT_THROW(
        CSVReader::read(file),
        std::runtime_error
    );

    std::remove(file.c_str());
}

TEST(CSVReaderTest, InvalidNumericValueThrows)
{
    const std::string file = "invalid_number.csv";

    std::ofstream out(file);

    out << "A,B\n";
    out << "1,abc\n";

    out.close();

    EXPECT_THROW(
        CSVReader::read(file),
        std::invalid_argument
    );

    std::remove(file.c_str());
}

TEST(CSVReaderTest, InconsistentColumnsThrow)
{
    const std::string file = "bad_columns.csv";

    std::ofstream out(file);

    out << "A,B,C\n";
    out << "1,2,3\n";
    out << "4,5\n";

    out.close();

    EXPECT_THROW(
        CSVReader::read(file),
        std::invalid_argument
    );

    std::remove(file.c_str());
}

TEST(CSVReaderTest, HeaderCountMismatchThrows)
{
    const std::string file = "header_mismatch.csv";

    std::ofstream out(file);

    out << "A,B,C,D\n";
    out << "1,2,3\n";

    out.close();

    EXPECT_THROW(
        CSVReader::read(file),
        std::invalid_argument
    );

    std::remove(file.c_str());
}

TEST(CSVReaderTest, ReadsSingleColumn)
{
    const std::string file = "single_column.csv";

    std::ofstream out(file);

    out << "Temperature\n";
    out << "10\n";
    out << "20\n";
    out << "30\n";

    out.close();

    Dataset dataset =
        CSVReader::read(file);

    EXPECT_EQ(dataset.getNumFeatures(), 1);
    EXPECT_EQ(dataset.getNumSamples(), 3);

    EXPECT_DOUBLE_EQ(dataset.getData()[2][0], 30);

    std::remove(file.c_str());
}

TEST(CSVReaderTest, ReadsSingleRow)
{
    const std::string file = "single_row.csv";

    std::ofstream out(file);

    out << "A,B,C\n";
    out << "5,6,7\n";

    out.close();

    Dataset dataset =
        CSVReader::read(file);

    EXPECT_EQ(dataset.getNumSamples(), 1);
    EXPECT_EQ(dataset.getNumFeatures(), 3);

    EXPECT_DOUBLE_EQ(dataset.getData()[0][2], 7);

    std::remove(file.c_str());
}

TEST(CSVReaderTest, ReadsNegativeNumbers)
{
    const std::string file = "negative.csv";

    std::ofstream out(file);

    out << "A,B\n";
    out << "-2,-5\n";

    out.close();

    Dataset dataset =
        CSVReader::read(file);

    EXPECT_DOUBLE_EQ(dataset.getData()[0][0], -2);
    EXPECT_DOUBLE_EQ(dataset.getData()[0][1], -5);

    std::remove(file.c_str());
}

TEST(CSVReaderTest, ReadsFloatingPointNumbers)
{
    const std::string file = "floating.csv";

    std::ofstream out(file);

    out << "A,B\n";
    out << "1.25,3.75\n";

    out.close();

    Dataset dataset =
        CSVReader::read(file);

    EXPECT_DOUBLE_EQ(dataset.getData()[0][0], 1.25);
    EXPECT_DOUBLE_EQ(dataset.getData()[0][1], 3.75);

    std::remove(file.c_str());
}

TEST(CSVReaderTest, SupportsCustomDelimiter)
{
    const std::string file = "semicolon.csv";

    std::ofstream out(file);

    out << "A;B\n";
    out << "1;2\n";
    out << "3;4\n";

    out.close();

    Dataset dataset =
        CSVReader::read(
            file,
            true,
            ';'
        );

    EXPECT_EQ(dataset.getNumSamples(), 2);
    EXPECT_EQ(dataset.getNumFeatures(), 2);

    EXPECT_DOUBLE_EQ(dataset.getData()[1][1], 4);

    std::remove(file.c_str());
}

TEST(CSVReaderTest, EmptyLinesAreIgnored)
{
    const std::string file = "empty_lines.csv";

    std::ofstream out(file);

    out << "A,B\n";
    out << "\n";
    out << "1,2\n";
    out << "\n";
    out << "3,4\n";

    out.close();

    Dataset dataset =
        CSVReader::read(file);

    EXPECT_EQ(dataset.getNumSamples(), 2);

    EXPECT_DOUBLE_EQ(dataset.getData()[1][0], 3);

    std::remove(file.c_str());
}

TEST(CSVReaderTest, ReadsLargeDataset)
{
    const std::string file = "large.csv";

    std::ofstream out(file);

    out << "A,B\n";

    for (int i = 0; i < 1000; i++)
    {
        out << i << "," << i * 2 << "\n";
    }

    out.close();

    Dataset dataset =
        CSVReader::read(file);

    EXPECT_EQ(dataset.getNumSamples(), 1000);
    EXPECT_EQ(dataset.getNumFeatures(), 2);

    EXPECT_DOUBLE_EQ(dataset.getData()[500][0], 500);
    EXPECT_DOUBLE_EQ(dataset.getData()[500][1], 1000);

    std::remove(file.c_str());
}