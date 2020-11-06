#include "error.hpp"
#include "k_delete.hpp"
#include "k_emplace.hpp"
#include "k_include.hpp"
#include "k_string.hpp"
#include "k_system.hpp"
#include "runtea.hpp"

// Runs tea string vector
void runTea(std::vector<std::string> &teafile, const char *&filename)
{
    // Defining tea value vectors
    teaString_t teaStrings;
    // Looping through tea file lines
    int line{0};
    for (std::string &prestatement : teafile)
    {
        ++line;
        // Comment encountered
        if (!prestatement.find('#'))
            continue;
        // Emplace keyword called
        else if (!prestatement.find("emplace "))
            kEmplace(prestatement, teaStrings);
        const std::string &statement{prestatement};

        // Delete keyword called
        if (!statement.find("delete "))
        {
            kDelete(statement, line, filename, teaStrings);
            continue;
        }
        // Include keyword called
        else if (!statement.find("include "))
        {
            kInclude(statement, line, filename);
            continue;
        }
        // String keyword called
        else if (!statement.find("string "))
        {
            kString(statement, line, filename, teaStrings);
            continue;
        }
        // System keyword called
        else if (!statement.find("system "))
        {
            kSystem(statement, line, filename);
            continue;
        }
        // Invalid statement
        else if (statement != "")
            teaSyntaxError(line, filename);
    }
}
