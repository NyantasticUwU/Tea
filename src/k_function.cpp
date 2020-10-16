#include "k_function.hpp"
#include "throwerror.hpp"
#include <algorithm>

// Defining static globals
static char endParamChar;
static std::string estatement;
static std::string funcName;
static std::string toArgs;
static std::vector<std::string> oldFunctionBody{};
static std::vector<std::string> funcBody{};
static std::vector<std::string> funcParams{};
static std::vector<std::string> funcArgs{};

// Parses single or no-parameter function declarations
static void parseLowParamCount(std::string &statement, std::vector<std::string> &funcParams)
{
    statement.erase(0, 1);
    statement.erase(0, statement.find_first_not_of(' '));
    if (statement.find_first_of(' ') < statement.find_first_of(')'))
        funcParams.push_back(statement.substr(0, statement.find_first_of(' ')));
    else if (statement[0] != ')')
        funcParams.push_back(statement.substr(0, statement.find_last_of(')')));
}

// Parses single or no-parameter function declarations
static void parseHighParamCount(std::string &statement, std::vector<std::string> &funcParams)
{
    statement.erase(0, 1);
    statement.erase(statement.size() - 1, statement.size());
    const int8_dynamic_t commaCount{
        static_cast<int8_dynamic_t>(std::count(statement.begin(), statement.end(), ','))};
    for (int8_dynamic_t i{0}; i < commaCount + 1; ++i)
    {
        statement.erase(0, statement.find_first_not_of(' '));
        endParamChar = i != commaCount ? ',' : ')';
        if (statement.find(' ') < statement.find(endParamChar))
            statement.erase(
                statement.find_first_of(' '),
                statement.find_first_of(endParamChar) - statement.find_first_of(' '));
        funcParams.push_back(statement.substr(0, statement.find_first_of(endParamChar)));
        if (i != commaCount)
            statement.erase(0, statement.find_first_of(endParamChar) + 1);
    }
}

// This function is called when the function keyword is used
void kFunction(std::string &statement, function_t &t_functions, function_t &t_functionParams,
               const std::vector<std::string> &parsedFile, int16_dynamic_t &index)
{
    if (std::count(statement.begin(), statement.end(), '(') < 1 ||
        std::count(statement.begin(), statement.end(), ')') < 1 ||
        std::find(statement.begin(), statement.end(), '(') > std::find(statement.begin(), statement.end(), ')'))
        throwError(statement);
    statement.erase(0, 8);
    statement.erase(0, statement.find_first_not_of(' '));
    if (statement.find_first_of('(') < statement.find_first_of(' '))
        funcName = statement.substr(0, statement.find_first_of('('));
    else
        funcName = statement.substr(0, statement.find_first_of(' '));
    if (funcName == "")
        return;
    while (index < parsedFile.size())
    {
        ++index;
        const std::string &newStatement{parsedFile[index]};
        if (newStatement.find("end_function") == 0)
            break;
        funcBody.push_back(newStatement);
    }
    t_functions.push_back(std::pair<std::string, std::vector<std::string>>{funcName, funcBody});
    statement.erase(0, statement.find_first_of('('));
    statement.erase(statement.find_last_of(')') + 1, statement.size());
    if (std::count(statement.begin(), statement.end(), ',') == 0)
        parseLowParamCount(statement, funcParams);
    else
        parseHighParamCount(statement, funcParams);
    t_functionParams.push_back(std::pair<std::string, std::vector<std::string>>{funcName, funcParams});
    funcBody.clear();
    funcParams.clear();
}

// This function is used to get all args from a function call
static std::vector<std::string> getArgs(std::string &statement)
{
    statement.erase(0, statement.find_first_of('('));
    statement = statement.substr(0, statement.find_last_of(')') + 1);
    statement.erase(0, 1);
    statement.erase(statement.find_last_of(')'), statement.size());
    const int8_dynamic_t commaCount{
        static_cast<int8_dynamic_t>(std::count(statement.begin(), statement.end(), ','))};
    std::vector<std::string> args{};
    if (commaCount == 0)
    {
        if (statement != "" ||
            !std::all_of(statement.begin(), statement.end(), [](const char &c) -> bool { return c == ' '; }))
            args.push_back(statement);
    }
    else
    {
        for (int8_dynamic_t i{0}; i < commaCount + 1; ++i)
        {
            endParamChar = i != commaCount ? ',' : ')';
            toArgs = statement.substr(0, statement.find_first_of(endParamChar));
            toArgs.erase(0, toArgs.find_first_not_of(' '));
            toArgs.erase(toArgs.find_last_not_of(' ') + 1, toArgs.size());
            args.push_back(toArgs);
            if (i != commaCount)
                statement.erase(0, statement.find_first_of(endParamChar) + 1);
        }
    }
    return args;
}

// This function is called when a tea function is encountered
void functionCalled(const std::string funcName, std::string &statement, std::vector<std::string> &functionBody,
                    const function_t &t_functionParams, std::vector<std::string> &parsedFile,
                    const int16_dynamic_t &index)
{
    if (std::count(statement.begin(), statement.end(), '(') < 1 ||
        std::count(statement.begin(), statement.end(), ')') < 1 ||
        std::find(statement.begin(), statement.end(), '(') > std::find(statement.begin(), statement.end(), ')'))
        throwError(statement);
    estatement = statement;
    funcArgs = getArgs(statement);
    int16_dynamic_t functionParamsIndex{0};
    for (const std::pair<std::string, std::vector<std::string>> &p : t_functionParams)
    {
        if (p.first == funcName)
        {
            if (funcArgs.size() != p.second.size())
                throwError(estatement);
            break;
        }
        ++functionParamsIndex;
    }
    oldFunctionBody = functionBody;
    for (int16_dynamic_t bodyIndex{0}; bodyIndex < functionBody.size(); ++bodyIndex)
    {
        std::string &fbstatement{functionBody[bodyIndex]};
        for (int16_dynamic_t paramIndex{0};
             paramIndex < t_functionParams[functionParamsIndex].second.size(); ++paramIndex)
        {
            const std::string &param{t_functionParams[functionParamsIndex].second[paramIndex]};
            while (fbstatement.find('{' + param + '}') != fbstatement.npos)
                fbstatement.replace(fbstatement.find('{' + param + '}'), param.size() + 2, funcArgs[paramIndex]);
        }
    }
    parsedFile.insert(parsedFile.begin() + index, functionBody.begin(), functionBody.end());
    parsedFile.erase(parsedFile.begin() + index + functionBody.size());
    functionBody = oldFunctionBody;
    funcArgs.clear();
    oldFunctionBody.clear();
}
