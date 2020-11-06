#include "createvar.hpp"
#include "error.hpp"
#include <algorithm>

// Defining static globals (hence the sg_ prefix)
// These are defined here for performance reasons
static constexpr char scg_validChars[64]{"0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_"};
static int sg_i;
static bool sg_isInString;
static int sg_spaceCount;
static int sg_statementSize;
static int sg_varnameSize;

// Checks for amount of spaces
static void checkSpaces(const std::string &statement, const int &line, const char *&filename, const int &kwlen,
                        int &secondSpaceIndex)
{
    sg_isInString = false;
    sg_spaceCount = 0;
    for (sg_i = kwlen; sg_i < sg_statementSize; ++sg_i)
    {
        if (statement[sg_i] == '"') // statement[sg_i - 1] will not be a backslash
        {
            sg_isInString = sg_isInString ? false : true;
            continue;
        }
        if (statement[sg_i] == '\\' && (statement[sg_i + 1] == '\\' || statement[sg_i + 1] == '"'))
        {
            ++sg_i;
            continue;
        }
        if (statement[sg_i] == ' ' && !sg_isInString) // Valid space
        {
            ++sg_spaceCount;
            if (sg_spaceCount == 2)
                secondSpaceIndex = sg_i;
        }
    }
    if (sg_spaceCount != 2)
        teaSyntaxError(line, filename);
}

// Gets variable name
static void getVarName(const std::string &statement, const int &kwlen, std::string &varname)
{
    varname.clear();
    for (sg_i = kwlen + 1; sg_i < sg_statementSize; ++sg_i)
    {
        if (statement[sg_i] == ' ')
            break;
        varname.push_back(statement[sg_i]);
    }
}

// Makes sure variable name is valid
static void validateVarName(const int &line, const char *&filename, const std::string &varname)
{
    if (std::none_of(std::begin(scg_validChars) + 10, std::end(scg_validChars),
                     [&](const char &c) -> const bool {
                         return c == varname[0];
                     }))
        teaSyntaxError(line, filename, "Invalid variable name.");
    sg_varnameSize = varname.size();
    for (sg_i = 1; sg_i < sg_varnameSize; ++sg_i)
    {
        if (std::none_of(std::begin(scg_validChars), std::end(scg_validChars),
                         [&](const char &c) -> const bool {
                             return c == varname[sg_i];
                         }))
            teaSyntaxError(line, filename, "Invalid variable name.");
    }
}

// Creates var
void createvar(const std::string &statement, const int &line, const char *&filename, const int &kwlen,
               std::string &varname, int &secondSpaceIndex)
{
    sg_statementSize = statement.size();
    checkSpaces(statement, line, filename, kwlen, secondSpaceIndex);
    getVarName(statement, kwlen, varname);
    validateVarName(line, filename, varname);
}
