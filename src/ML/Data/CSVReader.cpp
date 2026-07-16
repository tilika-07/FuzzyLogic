#include "ML/Data/CSVReader.h"
#include "ML/Data/Dataset.h"
#include <fstream>
#include <sstream>
#include <stdexcept>

Dataset CSVReader::read(
    const std::string& filename,
    bool hasHeader,
    char delimiter
)
{
    std::ifstream file(filename);

    if (!file.is_open())
    {
        throw std::runtime_error(
            "Unable to open CSV file: " + filename
        );
    }

    Matrix data;
    std::vector<std::string> headers;

    std::string line;

    //------------------------------------------------------
    // Read header (optional)
    //------------------------------------------------------

    if (hasHeader)
    {
        if (!std::getline(file, line))
        {
            throw std::runtime_error(
                "CSV file is empty."
            );
        }

        headers =
            parseHeader(
                line,
                delimiter
            );
    }
        //------------------------------------------------------
        // Read data
        //------------------------------------------------------

        size_t expectedColumns = 0;
        bool firstRow = true;

        size_t rowNumber =
            hasHeader ? 2 : 1;

        while (std::getline(file, line))
        {
            if (line.empty())
            {
                rowNumber++;
                continue;
            }

            Vector row =
                parseRow(
                    line,
                    delimiter
                );

            if (firstRow)
            {
                expectedColumns =
                    row.size();

                firstRow = false;
            }
            else if (row.size() != expectedColumns)
            {
                throw std::invalid_argument(
                    "Inconsistent number of columns at row "
                    + std::to_string(rowNumber)
                );
            }

            data.push_back(row);

            rowNumber++;
        }

        //------------------------------------------------------
        // Validation
        //------------------------------------------------------

        if (data.empty())
        {
            throw std::runtime_error(
                "CSV file contains no data."
            );
        }

        if (hasHeader &&
            headers.size() != expectedColumns)
        {
            throw std::invalid_argument(
                "Header count does not match data columns."
            );
        }

        //------------------------------------------------------
        // Construct Dataset
        //------------------------------------------------------

        return Dataset(
            data,
            headers
        );
    }


Vector CSVReader::parseRow(
    const std::string& line,
    char delimiter
)
{
    Vector row;

    std::stringstream stream(line);

    std::string token;

    while (std::getline(stream, token, delimiter))
    {
        try
        {
            row.push_back(
                std::stod(token)
            );
        }
        catch (const std::exception&)
        {
            throw std::invalid_argument(
                "Invalid numeric value: " + token
            );
        }
    }

    return row;
}
std::vector<std::string> CSVReader::parseHeader(
    const std::string& line,
    char delimiter
)
{
    std::vector<std::string> headers;

    std::stringstream stream(line);

    std::string token;

    while (std::getline(stream, token, delimiter))
    {
        headers.push_back(token);
    }

    return headers;
}