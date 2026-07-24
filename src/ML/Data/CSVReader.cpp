#include "ML/Data/CSVReader.h"
#include "ML/Data/Dataset.h"
#include <fstream>
#include <sstream>
#include <stdexcept>

Dataset CSVReader::read(
    const std::string& filename,
    bool hasHeader,
    bool lastColumnIsLabel,
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
    
    std::string line;
    std::vector<std::string> headers;
    if (hasHeader && std::getline(file, line))
    {
        headers = parseHeader(line, delimiter);
    }
    Matrix data;
    
    std::vector<std::string> labels;


       //read daata

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

            std::stringstream stream(line);

            std::string token;

            std::vector<std::string> tokens;

            while (std::getline(stream, token, delimiter))
            {
                tokens.push_back(token);
            }

            if (tokens.empty())
            {
                rowNumber++;
                continue;
            }

            if (lastColumnIsLabel)
            {
                labels.push_back(tokens.back());
                tokens.pop_back();
            }

            Vector row;

            for (const auto& value : tokens)
            {
                try
                {
                    row.push_back(std::stod(value));
                }
                catch (...)
                {
                    throw std::invalid_argument(
                        "Invalid numeric value: " + value
                    );
                }
            }

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
        //if header, read header

        size_t expectedHeaderColumns = expectedColumns;

        if (lastColumnIsLabel)
        {
            expectedHeaderColumns++;
        }

        if (hasHeader &&
            headers.size() != expectedHeaderColumns)
        {
            throw std::invalid_argument(
                "Header count does not match data columns."
            );
        }
       

        if (data.empty())
        {
            throw std::runtime_error(
                "CSV file contains no data."
            );
        }
        //construction
        if (lastColumnIsLabel && !headers.empty())
        {
            headers.pop_back();
        }
        return Dataset(
            data,
            headers,
            labels
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