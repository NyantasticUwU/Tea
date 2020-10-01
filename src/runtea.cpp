#include "k_string.hpp"
#include "k_system.hpp"
#include "runtea.hpp"
#include "throwerror.hpp"

// This function is used to emplace variables into the statement
static inline void emplaceVariables(std::string &statement,
    std::vector<std::pair<std::string, std::string>> &t_strings)
{
    for (std::pair<std::string, std::string> p : t_strings)
    {
        while (statement.find('{' + p.first + '}') != statement.npos)
            statement.replace(statement.find('{' + p.first + '}'), 2 + p.first.size(), p.second);
    }
}

// This function is used to run the main .tea (parsed) file
void runTea(std::vector<std::string> &parsedFile)
{
    std::system("cls");
    std::vector<std::pair<std::string, std::string>> t_strings{};
    for (std::string &statement : parsedFile)
    {
        emplaceVariables(statement, t_strings);
        if (statement.find("string") == 0)
        {
            kString(statement, t_strings);
            continue;
        }
        else if (statement.find("system") == 0)
        {
            kSystem(statement);
            continue;
        }
        else
            throwError(statement);
    }
}
