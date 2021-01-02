#include "constants.hpp"
#include "createvar.hpp"
#include "error.hpp"
#include <algorithm>
#include <vector>

// Defining static globals (hence the sg_ prefix)
// These are defined here for performance reasons
static constexpr char scg_validChars[64]{"0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_"};
static std::size_t sg_i;
static std::size_t sg_statementSize;

// Defining non-static globals
// These are used by outside files
int g_secondSpaceIndex;
std::string g_varname;

// Declaring globals
extern const char *g_teakeywords[TEA_NUMBER_OF_KEYWORDS];

// Checks for amount of spaces
static void checkSpaces(const std::string &statement, const int &line, const char *&filename, const int &kwlen)
{
    static bool s_isInString;
    static int s_spaceCount;
    s_isInString = false;
    s_spaceCount = 0;
    for (sg_i = kwlen; sg_i < sg_statementSize; ++sg_i)
    {
        if (statement[sg_i] == '"') // statement[sg_i - 1] will not be a backslash
        {
            s_isInString = s_isInString ? false : true;
            continue;
        }
        if (statement[sg_i] == '\\' && (statement[sg_i + 1U] == '\\' || statement[sg_i + 1U] == '"'))
        {
            ++sg_i;
            continue;
        }
        if (statement[sg_i] == ' ' && !s_isInString) // Valid space
        {
            ++s_spaceCount;
            if (s_spaceCount == 2)
                g_secondSpaceIndex = sg_i;
        }
    }
    if (s_spaceCount != 2)
        teaSyntaxError(line, filename);
}

// Gets variable name
static void getVarName(const std::string &statement, const int &kwlen)
{
    g_varname.clear();
    for (sg_i = kwlen + 1; sg_i < sg_statementSize; ++sg_i)
    {
        if (statement[sg_i] == ' ')
            break;
        g_varname.push_back(statement[sg_i]);
    }
}

// Makes sure variable name is valid
static void validateVarName(const int &line, const char *&filename)
{
    static std::size_t s_varnameSize;
    if (std::any_of(g_teakeywords, g_teakeywords + TEA_NUMBER_OF_KEYWORDS,
                    [&](const char *&str) noexcept -> const bool {
                        return str == g_varname;
                    }))
        teaSyntaxError(line, filename, "Variable name cannot be a keyword.");
    if (std::none_of(std::begin(scg_validChars) + 10, std::end(scg_validChars),
                     [&](const char &c) noexcept -> const bool {
                         return c == g_varname[0U];
                     }))
        teaSyntaxError(line, filename, "Invalid variable name.");
    s_varnameSize = g_varname.size();
    for (sg_i = 1U; sg_i < s_varnameSize; ++sg_i)
    {
        if (std::none_of(std::begin(scg_validChars), std::end(scg_validChars),
                         [&](const char &c) noexcept -> const bool {
                             return c == g_varname[sg_i];
                         }))
            teaSyntaxError(line, filename, "Invalid variable name.");
    }
}

// Creates var
void createvar(const std::string &statement, const int &line, const char *&filename, const int &kwlen)
{
    sg_statementSize = statement.size();
    checkSpaces(statement, line, filename, kwlen);
    getVarName(statement, kwlen);
    validateVarName(line, filename);
}
