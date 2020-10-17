#include "constants.hpp"
#include "specialfunctions.hpp"
#include "throwerror.hpp"
#include <algorithm>
#include <iostream>

// Used to get single parameter from tea function
static std::string extractSingleParam(std::string &statement)
{
    if (std::count(statement.begin(), statement.end(), '(') < 1 ||
        std::count(statement.begin(), statement.end(), ')') < 1 ||
        statement.find_first_of('(') > statement.find_first_of(')'))
        throwError(statement);
    statement.erase(0, statement.find_first_of('(') + 1);
    statement.erase(statement.find_last_of(')'), statement.size());
    return statement;
}

// Holds all special tea functions
namespace special_functions
{
    // Called when a special function is called in tea
    void specialFunctionCalled(std::string &statement, const int &funcIndex)
    {
        constants::specialFunctions[funcIndex].second(statement);
    }

    // Called when pause is called in tea
    void sf_pause(std::string &statement)
    {
        std::cout << extractSingleParam(statement);
        std::cin.ignore();
    }

    // Called when print is called in tea
    void sf_print(std::string &statement)
    {
        std::cout << extractSingleParam(statement);
    }

    // Called when printline is called in tea
    void sf_printline(std::string &statement)
    {
        std::cout << extractSingleParam(statement) << '\n';
    }
} // namespace special_functions
