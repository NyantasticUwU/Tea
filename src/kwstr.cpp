#include "error.hpp"
#include "kwstr.hpp"
#include <algorithm>

// Defining static globals
// These are defined here for performance reasons
static int g_statementSize;
static std::string g_content;
static int g_i;

// Checks if the string literal is closed
static void checkStringLiteral(const std::string &statement, const int &line, const int &kwlen)
{
    if (std::count_if(statement.begin() + (kwlen + 1), statement.end(),
                      [](const char &c) -> bool { return c == '"' && *(&c - 1) != '\\'; }) != 2)
        teaSyntaxError(line, "String literal must be closed off.");
}

// Loops through each character in the string literal and adds it to the command string
static void getContent(const std::string &statement, const int &line, const int &kwlen)
{
    g_content.clear();
    for (g_i = kwlen + 2; g_i < g_statementSize; ++g_i)
    {
        if (statement[g_i] == '"') // statement[g_i - 1] will not be a backslash
        {
            if (g_i + 1 != g_statementSize) // If statement has trailing characters
                teaSyntaxError(line, "No characters are allowed after string literal.");
            break;
        }
        if (statement[g_i] == '\\' && statement[g_i + 1] == '"')
        {
            g_content.push_back('"');
            ++g_i;
            continue;
        }
        g_content.push_back(statement[g_i]);
    }
}

// Extracts chars inside string literal and puts it in command
std::string &getStringLiteral(const std::string &statement, const int &line, const int &kwlen)
{
    if (statement.size() < 9)
        teaSyntaxError(line);
    if (statement[kwlen + 1] != '"')
        teaSyntaxError(line, "String literal required here.");
    g_statementSize = statement.size();
    checkStringLiteral(statement, line, kwlen);
    getContent(statement, line, kwlen);
    return g_content;
}
