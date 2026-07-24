#include "ML/Data/CSVWriter.h"
void CSVWriter::write(
    const std::string& filename,
    const Dataset& dataset,
    char delimiter,
    bool lastColumnIsLabel
)
{
    const Matrix& data =
        dataset.getData();

    const auto& headers =
        dataset.getFeatureNames();

    //validation

    if (data.empty())
    {
        throw std::invalid_argument(
            "Cannot write an empty dataset."
        );
    }

    if (data[0].empty())
    {
        throw std::invalid_argument(
            "Dataset contains no features."
        );
    }

    size_t numColumns =
        data[0].size();

    for (const auto& row : data)
    {
        if (row.size() != numColumns)
        {
            throw std::invalid_argument(
                "All rows must have the same number of columns."
            );
        }
    }

    if (!headers.empty() &&
        headers.size() != numColumns)
    {
        throw std::invalid_argument(
            "Header count does not match number of columns."
        );
    }
    //open
    std::ofstream file(filename);

    if (!file.is_open())
    {
        throw std::runtime_error(
            "Unable to open file: " + filename
        );
    }
    file << std::setprecision(
        std::numeric_limits<double>::max_digits10
    );
//if headers, write headers
    if (!headers.empty())
    {
        for (size_t i = 0; i < headers.size(); i++)
        {
            file << headers[i];

            if (i + 1 != headers.size())
            {
                file << delimiter;
            }
        }

        file << '\n';
    }


    for (const auto& row : data)
    {
        for (size_t i = 0; i < row.size(); i++)
        {
            
            file << row[i];

            if (i + 1 != row.size())
            {
                file << delimiter;
            }
        }

        file << '\n';
    }


    if (!file)
    {
        throw std::runtime_error(
            "Failed while writing CSV file."
        );
    }
}