#pragma once
#include <iomanip>
#include <fstream>
#include <limits>
#include <stdexcept>
#include "ML/Data/Dataset.h"

class CSVWriter
{
public:

    static void write(
        const std::string& filename,
        const Dataset& dataset,
        char delimiter = ','
    );
};