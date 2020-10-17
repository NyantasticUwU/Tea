#include "k_function.hpp"
#include "k_include.hpp"
#include "k_string.hpp"
#include "k_system.hpp"
#include "runtea.hpp"
#include "specialfunctions.hpp"
#include "throwerror.hpp"

// Defining static globals
static bool functionIsCalled;
static bool specialFunctionIsCalled;

// This function is used to emplace variables into the statement
static inline void emplaceVariables(std::string &statement,
                                    const std::vector<std::pair<std::string, std::string>> &t_strings)
{
    for (const std::pair<std::string, std::string> &p : t_strings)
    {
        while (statement.find('{' + p.first + '}') != statement.npos)
            statement.replace(statement.find('{' + p.first + '}'), p.first.size() + 2, p.second);
    }
}

// This function is used to run the main .tea (parsed) file
void runTea(std::vector<std::string> &parsedFile)
{
    std::system("cls");
    std::vector<std::pair<std::string, std::string>> t_strings{};
    function_t t_functions{};
    function_t t_functionParams{};
    for (int16_dynamic_t index{0}; index < parsedFile.size(); ++index)
    {
        std::string &statement{parsedFile[index]};
        emplaceVariables(statement, t_strings);
        if (statement.find("include") == 0)
        {
            kInclude(statement, parsedFile, index);
            --index;
            continue;
        }
        else if (statement.find("string") == 0)
        {
            kString(statement, t_strings);
            continue;
        }
        else if (statement.find("system") == 0)
        {
            kSystem(statement);
            continue;
        }
        else if (statement.find("function") == 0)
        {
            kFunction(statement, t_functions, t_functionParams, parsedFile, index);
            continue;
        }
        else
        {
            functionIsCalled = false;
            for (std::pair<std::string, std::vector<std::string>> &f : t_functions)
            {
                if (statement.find(f.first) == 0)
                {
                    functionIsCalled = true;
                    functionCalled(f.first, statement, f.second, t_functionParams, parsedFile, index);
                    --index;
                }
            }
            if (functionIsCalled)
                continue;
            specialFunctionIsCalled = false;
            int funcIndex{0};
            for (const std::pair<std::string, std::function<void(std::string &)>> &p : constants::specialFunctions)
            {
                if (statement.find(p.first) == 0 && statement[p.first.size()] == ' ' ||
                    statement[p.first.size()] == '(')
                {
                    specialFunctionIsCalled = true;
                    special_functions::specialFunctionCalled(statement, funcIndex);
                }
                ++funcIndex;
            }
            if (specialFunctionIsCalled)
                continue;
            throwError(statement);
        }
    }
}
