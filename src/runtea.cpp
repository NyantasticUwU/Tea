#include "error.hpp"
#include "evalops.hpp"
#include "k_assign.hpp"
#include "k_delete.hpp"
#include "k_elif.hpp"
#include "k_else.hpp"
#include "k_emplace.hpp"
#include "k_exit.hpp"
#include "k_float.hpp"
#include "k_if.hpp"
#include "k_import.hpp"
#include "k_include.hpp"
#include "k_int.hpp"
#include "k_string.hpp"
#include "k_system.hpp"
#include "k_while.hpp"
#include "runtea.hpp"

// Checks if statement starts with the given keyword
const bool startsWithKeyword(const std::string &statement, const char *teaKeyword) noexcept
{
    static std::size_t s_i;
    static std::size_t s_statementSize;
    s_statementSize = statement.size();
    for (s_i = 0U; s_i < s_statementSize; ++s_i)
    {
        if (teaKeyword[s_i] == '\0')
            return true;
        if (teaKeyword[s_i] != statement[s_i])
            return false;
    }
    return false;
}

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
    int line{0};
    loopTeaStatements(teafile, line, filename, teaStrings, teaInts, teaFloats);
}

// Runs tea statement
void loopTeaStatements(std::vector<std::string> &teafile, int &line, const char *&filename,
                       teaString_t &teaStrings, teaInt_t &teaInts, teaFloat_t &teaFloats)
{
    // Looping through tea file lines
    const int teafileSize{static_cast<int>(teafile.size())};
    std::string prestatement;
    while (line < teafileSize)
    {
        prestatement = teafile[line];
        ++line;
        // Comment encountered
        if (prestatement[0U] == '#')
            continue;
        // Emplace keyword called
        else if (startsWithKeyword(prestatement, "emplace "))
            kEmplace(prestatement, teaStrings, teaInts, teaFloats);
        evalOps(prestatement, line, filename);
        const std::string &statement{prestatement};

        // Assign keyword called
        if (startsWithKeyword(statement, "assign "))
        {
            kAssign(statement, line, filename, teaStrings, teaInts, teaFloats);
            continue;
        }
        // Delete keyword called
        else if (startsWithKeyword(statement, "delete "))
        {
            kDelete(statement, line, filename, teaStrings, teaInts, teaFloats);
            continue;
        }
        // Elif keyword called
        else if (startsWithKeyword(statement, "elif"))
        {
            if (kElif(line, teafile, teafileSize))
                return;
        }
        // Else keyword called
        else if (statement == "else")
        {
            if (kElse(line, teafile, teafileSize))
                return;
        }
        // End keyword called
        else if (statement == "end")
            return;
        // Exit keyword called
        else if (startsWithKeyword(statement, "exit "))
        {
            kExit(statement, line, filename);
            continue;
        }
        // Float keyword called
        else if (startsWithKeyword(statement, "float "))
        {
            kFloat(statement, line, filename, teaFloats);
            continue;
        }
        // If keyword called
        else if (startsWithKeyword(statement, "if "))
        {
            kIf(teafile, teafileSize, statement, line, filename, teaStrings, teaInts, teaFloats);
            continue;
        }
        // Import keyword called
        else if (startsWithKeyword(statement, "import "))
        {
            kImport(statement, line, filename, teaStrings, teaInts, teaFloats);
            continue;
        }
        // Include keyword called
        else if (startsWithKeyword(statement, "include "))
        {
            kInclude(statement, line, filename);
            continue;
        }
        // Int keyword called
        else if (startsWithKeyword(statement, "int "))
        {
            kInt(statement, line, filename, teaInts);
            continue;
        }
        // String keyword called
        else if (startsWithKeyword(statement, "string "))
        {
            kString(statement, line, filename, teaStrings);
            continue;
        }
        // System keyword called
        else if (startsWithKeyword(statement, "system "))
        {
            kSystem(statement, line, filename);
            continue;
        }
        // While keyword called
        else if (startsWithKeyword(statement, "while "))
        {
            kWhile(teafile, teafileSize, statement, teafile[line - 1], line, filename, teaStrings, teaInts,
                   teaFloats);
            continue;
        }
        // Invalid statement
        else if (statement != "")
            teaSyntaxError(line, filename);
    }
}
