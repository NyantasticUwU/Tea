#include "error.hpp"
#include "evalops.hpp"
#include "k_delete.hpp"
#include "k_emplace.hpp"
#include "k_float.hpp"
#include "k_import.hpp"
#include "k_include.hpp"
#include "k_int.hpp"
#include "k_string.hpp"
#include "k_system.hpp"
#include "runtea.hpp"

// Runs tea string vector
void runTea(std::vector<std::string> &teafile, const char *&filename, const teaString_t *const &pteaStrings,
            const teaInt_t *const &pteaInts, const teaFloat_t *const &pteaFloats)
{
    // Defining tea value vectors
    teaFloat_t teaFloats;
    if (pteaFloats)
        teaFloats = *pteaFloats;
    teaInt_t teaInts;
    if (pteaInts)
        teaInts = *pteaInts;
    teaString_t teaStrings;
    if (pteaStrings)
        teaStrings = *pteaStrings;
    // Looping through tea file lines
    const int teafileSize{static_cast<int>(teafile.size())};
    for (int line{0}; line < teafileSize; /* line is incremented elsewhere */)
    {
        std::string &prestatement{teafile[line]};
        ++line;
        // Comment encountered
        if (!prestatement.find('#'))
            continue;
        // Emplace keyword called
        else if (!prestatement.find("emplace "))
            kEmplace(prestatement, teaStrings, teaInts, teaFloats);
        evalOps(prestatement, line, filename);
        const std::string &statement{prestatement};

        // Delete keyword called
        if (!statement.find("delete "))
        {
            kDelete(statement, line, filename, teaStrings, teaInts, teaFloats);
            continue;
        }
        // Float keyword called
        else if (!statement.find("float "))
        {
            kFloat(statement, line, filename, teaFloats);
            continue;
        }
        // Import keyword called
        else if (!statement.find("import "))
        {
            kImport(statement, line, filename, teaStrings, teaInts, teaFloats);
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
