#include "error.hpp"
#include "kwstr.hpp"
#include <algorithm>

// Defining static globals
// These are defined here for performance reasons
static int statementSize;
static std::string content;
static int i;
static int lastDQIndex;

// Checks if the string literal is closed
static void checkStringLiteral(const std::string &statement, const int &line, const int &kwlen)
{
    if (std::count_if(statement.begin() + (kwlen + 1), statement.end(),
                      [](const char &c) -> bool { return c == '"' && *(&c - 1) != '\\'; }) != 2)
        teaSyntaxError(line, "String literal must be closed off.");
}

// Loops through each character in the string literal and adds it to the command string
static void getContent(const std::string &statement, const int &kwlen)
{
    content.clear();
    for (i = kwlen + 2; i < statementSize; ++i)
    {
        if (statement[i] == '"') // statement[i - 1] will not be a backslash
        {
            lastDQIndex = i;
            break;
        }
        if (statement[i] == '\\' && statement[i + 1] == '"')
        {
            content.push_back('"');
            ++i;
            continue;
        }
        content.push_back(statement[i]);
    }
}

// Checks for any arbitrary characters after string literal
static void checkRestOfStatement(const int &line)
{
    if (lastDQIndex + 1 != statementSize)
        teaSyntaxError(line, "No characters are allowed after string literal.");
}

// Extracts chars inside string literal and puts it in command
std::string &getStringLiteral(const std::string &statement, const int &line, const int &kwlen)
{
    if (statement.size() < 9)
        teaSyntaxError(line);
    if (statement[kwlen + 1] != '"')
        teaSyntaxError(line, "String literal required here.");
    statementSize = statement.size();
    checkStringLiteral(statement, line, kwlen);
    getContent(statement, kwlen);
    checkRestOfStatement(line);
    return content;
}
