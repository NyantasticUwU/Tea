#include "parsefile.hpp"
#include <assert.h>
#include <fstream>
#include <vector>

// This function creates a statement vector from the file contents
static std::vector<std::string> createStatementVector(const std::string &filecontents)
{
    std::string statement{""};
    std::vector<std::string> statementVector{};
    for (const char &c : filecontents)
    {
        statement += c;
        if (c == '\n')
        {
            statementVector.push_back(statement);
            statement = "";
        }
    }
    statementVector.push_back(statement);
    return statementVector;
}

// This function is used to remove whitespace at the beginning and end of all vector statements
static void removeWhitespace(std::vector<std::string> &statementVector)
{
    for (std::string &statement : statementVector)
    {
        statement.erase(0, statement.find_first_not_of(' '));
        if (statement.find(';') != statement.npos)
            statement.erase(statement.find_last_of(';') + 1, statement.size());
    }
}

// This function is used to remove any non valid statements from the vector
static std::vector<std::string> removeNonValidStatements(std::vector<std::string> &statementVector)
{
    std::vector<std::string> validStatementVector{};
    for (const std::string &statement : statementVector)
    {
        if (statement.find(';') != statement.npos)
            validStatementVector.push_back(statement);
    }
    return validStatementVector;
}

// This function is used to remove any extra whitespace from the given file
std::vector<std::string> parseFile(char *&filename)
{
    assert(std::ifstream{filename}.good() && "File is non-existent.");
    std::ifstream f{filename};
    const std::string filecontents{std::istreambuf_iterator<char>{f}, std::istreambuf_iterator<char>{}};
    f.close();
    std::vector<std::string> statementVector{createStatementVector(filecontents)};
    removeWhitespace(statementVector);
    return removeNonValidStatements(statementVector);
}
