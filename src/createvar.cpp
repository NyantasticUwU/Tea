#include "constants.hpp"
#include "createvar.hpp"
#include "error.hpp"
#include <algorithm>
#include <vector>

// Defining static globals (hence the sg_ prefix)
// These are defined here for performance reasons
static constexpr char scg_validChars[64]{"0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_"};

// Defining non-static globals
// These are used by outside files
int g_secondSpaceIndex;
std::string g_varname;

// Declaring globals
extern const char *g_teakeywords[TEA_NUMBER_OF_KEYWORDS];

// Checks for amount of spaces
static void checkSpaces(const std::string &statement, const std::size_t &statementSize, const int &line,
                        const char *&filename, const int &kwlen)
{
    bool &&isInString{false};
    int &&spaceCount{0};
    for (std::size_t &&i{static_cast<std::size_t>(kwlen)}; i < statementSize; ++i)
    {
        if (statement[i] == '"') // statement[i - 1] will not be a backslash
        {
            isInString = isInString ? false : true;
            continue;
        }
        if (statement[i] == '\\' && (statement[i + 1U] == '\\' || statement[i + 1U] == '"'))
        {
            ++i;
            continue;
        }
        if (statement[i] == ' ' && !isInString) // Valid space
        {
            ++spaceCount;
            if (spaceCount == 2)
                g_secondSpaceIndex = i;
        }
    }
    if (spaceCount != 2)
        teaSyntaxError(line, filename);
}

// Gets variable name
static void getVarName(const std::string &statement, const std::size_t &statementSize, const int &kwlen)
{
    g_varname.clear();
    for (std::size_t &&i{static_cast<std::size_t>(kwlen + 1)}; i < statementSize; ++i)
    {
        if (statement[i] == ' ')
            break;
        g_varname.push_back(statement[i]);
    }
}

// Makes sure variable name is valid
static void validateVarName(const int &line, const char *&filename)
{
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
    const std::size_t &&varnameSize{g_varname.size()};
    for (std::size_t &&i{1U}; i < varnameSize; ++i)
    {
        if (std::none_of(std::begin(scg_validChars), std::end(scg_validChars),
                         [&](const char &c) noexcept -> const bool {
                             return c == g_varname[i];
                         }))
            teaSyntaxError(line, filename, "Invalid variable name.");
    }
}

// Creates var
void createvar(const std::string &statement, const int &line, const char *&filename, const int &kwlen)
{
    const std::size_t &&statementSize{statement.size()};
    checkSpaces(statement, statementSize, line, filename, kwlen);
    getVarName(statement, statementSize, kwlen);
    validateVarName(line, filename);
}
