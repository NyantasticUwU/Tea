#include "error.hpp"
#include "k_system.hpp"
#include <algorithm>

// Defining static globals
// These are defined here for performance reasons
static int statementSize;
static std::string command;
static int i;
static int lastDQIndex;

// Throws ERROR_INVALID_DATA error with custom message
static void kSystemError(const int &line, const std::string &msg = "")
{
    teaError("ERROR: Invalid statement on line " + std::to_string(line) + ".\n" +
                 msg + (msg == "" ? "" : "\n") +
                 "Exiting with code 13 (ERROR_INVALID_DATA).",
             13); // Windows' ERROR_INVALID_DATA (13)
}

// Checks if the string literal is closed
static void checkStringLiteral(const std::string &statement, const int &line)
{
    if (std::count_if(statement.begin() + 7, statement.end(),
                      [](const char &c) -> bool { return c == '"' && *(&c - 1) != '\\'; }) != 2)
        kSystemError(line, "String literal must be closed off.");
}

// Loops through each character in the string literal and adds it to the command string
static void getCommand(const std::string &statement)
{
    command = "";
    for (i = 8; i < statementSize; ++i)
    {
        if (statement[i] == '"') // statement[i - 1] will not be a backslash
        {
            lastDQIndex = i;
            break;
        }
        if (statement[i] == '\\' && statement[i + 1] == '"')
        {
            command += '"';
            ++i;
            continue;
        }
        command += statement[i];
    }
}

// Checks for any arbitrary characters after string literal
static void checkRestOfStatement(const int &line)
{
    if (lastDQIndex + 1 != statementSize)
        kSystemError(line, "No characters are allowed after string literal.");
}

// Called when the system keyword is called in tea
void kSystem(const std::string &statement, const int &line)
{
    if (statement[6] != ' ')
        kSystemError(line, "A single space is required after any keyword.");
    if (statement[7] != '"')
        kSystemError(line, "System keyword requires a string literal.");
    statementSize = statement.size();
    checkStringLiteral(statement, line);
    getCommand(statement);
    checkRestOfStatement(line);
    std::system(command.c_str());
}
