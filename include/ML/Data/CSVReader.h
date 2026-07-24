#pragma once
#include "ML/Data/Dataset.h"
#include <string>
#include <vector>
using Vector = std::vector<double>;
using Matrix = std::vector<Vector>;
class CSVReader
{
public:
    static Dataset read(
        const std::string& filename,
        bool hasHeader = true,
        bool lastColumnIsLabel=false,
        char delimiter = ','
    );
private:
    static Vector parseRow(
        const std::string& line,
        char delimiter
    );
    static std::vector<std::string> parseHeader(
        const std::string& line,
        char delimiter
    );
};