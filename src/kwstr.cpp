#include "error.hpp"
#include "kwstr.hpp"

// Defining static globals (hence the sg_ prefix)
// These are defined here for performance reasons
static std::string sg_content;
static int sg_i;
static int sg_quoteCount;
static int sg_statementSize;

// Checks if the string literal is closed
static void checkStringLiteral(const std::string &statement, const int &line, const int &kwlen)
{
    sg_quoteCount = 0;
    for (sg_i = kwlen + 1; sg_i < sg_statementSize; ++sg_i)
    {
        if (statement[sg_i] == '"') // statement[sg_i - 1] will not be a backslash
        {
            ++sg_quoteCount;
            continue;
        }
        if (statement[sg_i] == '\\' && (statement[sg_i + 1] == '\\' || statement[sg_i + 1] == '"'))
        {
            ++sg_i;
            continue;
        }
    }
    if (sg_quoteCount != 2)
        teaSyntaxError(line, "String literal must be closed off.");
}

// Loops through each character in the string literal and adds it to the command string
static void getContent(const std::string &statement, const int &line, const int &kwlen)
{
    sg_content.clear();
    for (sg_i = kwlen + 2; sg_i < sg_statementSize; ++sg_i)
    {
        if (statement[sg_i] == '"') // statement[sg_i - 1] will not be a backslash
        {
            if (sg_i + 1 != sg_statementSize) // If statement has trailing characters
                teaSyntaxError(line, "No characters are allowed after string literal.");
            break;
        }
        if (statement[sg_i] == '\\' && statement[sg_i + 1] == '\\')
        {
            sg_content.push_back('\\');
            ++sg_i;
            continue;
        }
        if (statement[sg_i] == '\\' && statement[sg_i + 1] == '"')
        {
            sg_content.push_back('"');
            ++sg_i;
            continue;
        }
        sg_content.push_back(statement[sg_i]);
    }
}

// Extracts chars inside string literal and puts it in command
std::string &getStringLiteral(const std::string &statement, const int &line, const int &kwlen)
{
    if (statement.size() < 9)
        teaSyntaxError(line);
    if (statement[kwlen + 1] != '"')
        teaSyntaxError(line, "String literal required here.");
    sg_statementSize = statement.size();
    checkStringLiteral(statement, line, kwlen);
    getContent(statement, line, kwlen);
    return sg_content;
}
