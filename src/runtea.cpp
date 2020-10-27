#include "error.hpp"
#include "runtea.hpp"
#include <algorithm>

// Runs tea string vector
void runTea(std::vector<std::string> &teafile)
{
    // Looping through tea file lines
    int line{0};
    for (std::string &statement : teafile)
    {
        ++line;
        // Comment encountered
        if (!statement.find('#'))
            continue;
        // Invalid statement
        else if (std::any_of(statement.begin(), statement.end(), [](const char &c) -> bool { return c != ' '; }))
            teaError("ERROR: Invalid statement on line " + std::to_string(line) + ".\n" +
                         "Exiting with code 13 (ERROR_INVALID_DATA).",
                     13); // Windows' ERROR_INVALID_DATA (13)
    }
}
