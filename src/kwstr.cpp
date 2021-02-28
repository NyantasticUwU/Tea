#include "error.hpp"
#include "kwstr.hpp"
#include "stringsupport.hpp"

// Defining static globals (hence the sg_ prefix)
// These are defined here for performance reasons
static std::string sg_content;

// Checks if the string literal is closed
static inline void checkStringLiteral(const std::string &statement, const std::size_t &statementSize,
    const int &line, const char *&filename, const int &kwlen)
{
    int &&quoteCount{0};
    for (std::size_t &&i{static_cast<std::size_t>(kwlen + 1)}; i < statementSize; ++i)
    {
        if (statement[i] == '"') // statement[i - 1] will not be a backslash
        {
            ++quoteCount;
            continue;
        }
        if (statement[i] == '\\' && (statement[i + 1U] == '\\' || statement[i + 1U] == '"'))
        {
            ++i;
            continue;
        }
    }
    if (quoteCount != 2)
        teaSyntaxError(line, filename, "String literal must be closed off.");
}

// Loops through each character in the string literal and adds it to the command string
static inline void getContent(const std::string &statement, const std::size_t &statementSize, const int &kwlen)
{
    sg_content.clear();
    for (std::size_t &&i{static_cast<std::size_t>(kwlen + 2)}; i < statementSize; ++i)
        sg_content.push_back(statement[i]);
    formatTeaString(sg_content);
}

// Extracts chars inside string literal and puts it in command
std::string &getStringLiteral(const std::string &statement, const int &line, const char *&filename,
    const int &kwlen)
{
    if (statement.size() < 9U)
        teaSyntaxError(line, filename);
    if (statement[kwlen + 1] != '"')
        teaSyntaxError(line, filename, "String literal required here.");
    const std::size_t &&statementSize{statement.size()};
    checkStringLiteral(statement, statementSize, line, filename, kwlen);
    getContent(statement, statementSize - 1U, kwlen);
    return sg_content;
}
