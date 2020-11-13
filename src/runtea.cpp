#include "error.hpp"
#include "k_delete.hpp"
#include "k_emplace.hpp"
#include "k_include.hpp"
#include "k_int.hpp"
#include "k_string.hpp"
#include "k_system.hpp"
#include "runtea.hpp"

// Runs tea string vector
void runTea(std::vector<std::string> &teafile, const char *&filename)
{
    // Defining tea value vectors
    teaInt_t teaInts;
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
            kEmplace(prestatement, teaStrings, teaInts);
        const std::string &statement{prestatement};

        // Delete keyword called
        if (!statement.find("delete "))
        {
            kDelete(statement, line, filename, teaStrings, teaInts);
            continue;
        }
        // Include keyword called
        else if (!statement.find("include "))
        {
            kInclude(statement, line, filename);
            continue;
        }
        // Int keyword called
        else if (!statement.find("int "))
        {
            kInt(statement, line, filename, teaInts);
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
