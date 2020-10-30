#include "error.hpp"
#include "k_system.hpp"
#include "runtea.hpp"
#include <algorithm>

// Runs tea string vector
void runTea(std::vector<std::string> &teafile)
{
    // Looping through tea file lines
    int line{0};
    for (const std::string &statement : teafile)
    {
        ++line;
        // Comment encountered
        if (!statement.find('#'))
            continue;
        // System keyword called
        else if (!statement.find("system "))
        {
            kSystem(statement, line);
            continue;
        }
        // Invalid statement
        else if (statement != "")
            teaError("ERROR: Invalid statement on line " + std::to_string(line) + ".\n" +
                         "Exiting with code 13 (ERROR_INVALID_DATA).",
                     13); // Windows' ERROR_INVALID_DATA (13)
    }
}
