#include "constants.hpp"
#include "specialfunctions.hpp"
#include "throwerror.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>

// Defining static globals
static std::string param;
static std::vector<std::string> params{};
static std::ofstream f;

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

// Used to get multiple parameters from tea function
static void extractMultipleParams(std::string &statement, const int8_dynamic_t numberOfParams)
{
    if (std::count(statement.begin(), statement.end(), '(') < 1 ||
        std::count(statement.begin(), statement.end(), ')') < 1 ||
        statement.find_first_of('(') > statement.find_first_of(')') ||
        std::count(statement.begin(), statement.end(), ',') != numberOfParams - 1)
        throwError(statement);
    statement.erase(0, statement.find_first_of('(') + 1);
    for (int8_dynamic_t i{0}; i < numberOfParams; ++i)
    {
        statement.erase(0, statement.find_first_not_of(' '));
        if (i + 1 != numberOfParams)
        {
            param = statement.substr(0, statement.find_first_of(','));
            param.erase(param.find_last_not_of(' ') + 1, param.size());
            params.push_back(param);
            statement.erase(0, statement.find_first_of(',') + 1);
        }
        else
        {
            param = statement.substr(0, statement.find_first_of(')'));
            param.erase(param.find_last_not_of(' ') + 1, param.size());
            params.push_back(param);
        }
    }
}

// Holds all special tea functions
namespace special_functions
{
    // Called when a special function is called in tea
    void specialFunctionCalled(std::string &statement, const int &funcIndex)
    {
        constants::specialFunctions[funcIndex].second(statement);
    }

    // Called when faout is called in tea
    void sf_faout(std::string &statement)
    {
        extractMultipleParams(statement, 2);
        f.open(params[0], std::ios_base::app);
        f << params[1];
        f.close();
        params.clear();
    }

    // Called when fout is called in tea
    void sf_fout(std::string &statement)
    {
        extractMultipleParams(statement, 2);
        f.open(params[0]);
        f << params[1];
        f.close();
        params.clear();
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
