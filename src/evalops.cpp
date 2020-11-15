#include "error.hpp"
#include "evalops.hpp"
#include <algorithm>

// Defining static globals (hence the sg_ prefix)
// These are defined here for performance reasons
static constexpr char scg_validNumerics[14]{"0123456789.xX"};
static int sg_i;
static bool sg_isInString;
static int sg_leftOperatorStartIndex; // Indicates start of left operand
static int sg_operatorIndex;
static int sg_statementSize;

// Searches for operator
static int searchOperator(const std::string &statement, const std::string &op)
{
    if (statement.find(op) == statement.npos)
        return statement.npos;
    sg_isInString = false;
    sg_statementSize = statement.size();
    for (sg_i = 0; sg_i < sg_statementSize; ++sg_i)
    {
        if (statement[sg_i] == '"') // statement[sg_i - 1] will not be a backslash
        {
            sg_isInString = sg_isInString ? false : true;
            if (sg_isInString)
                sg_leftOperatorStartIndex = sg_i;
            continue;
        }
        if (statement[sg_i] == '\\' && (statement[sg_i + 1] == '\\' || statement[sg_i + 1] == '"'))
        {
            ++sg_i;
            continue;
        }
        if (!sg_isInString && statement.substr((sg_i > 0 ? sg_i - 1 : 0), op.size() + 2) == ' ' + op + ' ')
            return sg_i;
    }
    return statement.npos;
}

// Gets left operand
static void getLeftOperand(const std::string &statement, std::string &leftOperand)
{
    for (sg_i = sg_leftOperatorStartIndex + 1; sg_i < sg_operatorIndex - 2; ++sg_i)
        leftOperand.push_back(statement[sg_i]);
}
// Gets left operand
static void getLeftOperand(const std::string &statement, int &leftOperand)
{
    std::string intstr;
    for (sg_i = sg_leftOperatorStartIndex + 1; sg_i < sg_operatorIndex - 1; ++sg_i)
        intstr.push_back(statement[sg_i]);
    leftOperand = std::stoi(intstr);
}
// Gets left operand
static void getLeftOperand(const std::string &statement, float &leftOperand)
{
    std::string floatstr;
    for (sg_i = sg_leftOperatorStartIndex + 1; sg_i < sg_operatorIndex - 1; ++sg_i)
        floatstr.push_back(statement[sg_i]);
    leftOperand = std::stof(floatstr);
}

// Gets right operand
static void getRightOperand(const std::string &statement, std::string &rightOperand)
{
    sg_statementSize = statement.size();
    for (sg_i = sg_operatorIndex + 3; sg_i < sg_statementSize; ++sg_i)
    {
        if (statement[sg_i] == '"') // statement[sg_i - 1] will not be a backslash
            break;
        if (statement[sg_i] == '\\' && (statement[sg_i + 1] == '\\' || statement[sg_i + 1] == '"'))
        {
            rightOperand.push_back(statement[sg_i]);
            ++sg_i;
            rightOperand.push_back(statement[sg_i]);
            continue;
        }
        rightOperand.push_back(statement[sg_i]);
    }
}
// Gets right operand
static void getRightOperand(const std::string &statement, int &rightOperand)
{
    sg_statementSize = statement.size();
    std::string intstr;
    for (sg_i = sg_operatorIndex + 2; sg_i < sg_statementSize; ++sg_i)
    {
        if (std::none_of(std::begin(scg_validNumerics),
                         std::end(scg_validNumerics),
                         [&](const char &c) noexcept -> const bool {
                             return c == statement[sg_i];
                         }))
            break;
        intstr.push_back(statement[sg_i]);
    }
    rightOperand = std::stoi(intstr, nullptr, 0);
}
// Gets right operand
static void getRightOperand(const std::string &statement, float &rightOperand)
{
    sg_statementSize = statement.size();
    std::string floatstr;
    for (sg_i = sg_operatorIndex + 2; sg_i < sg_statementSize; ++sg_i)
    {
        if (std::none_of(std::begin(scg_validNumerics),
                         std::end(scg_validNumerics),
                         [&](const char &c) noexcept -> const bool {
                             return c == statement[sg_i];
                         }))
            break;
        floatstr.push_back(statement[sg_i]);
    }
    rightOperand = std::stof(floatstr);
}

// Checks if right operand is int or float
// Returns true if int false if float
static bool isROIntOrFloat(const std::string &statement)
{
    sg_statementSize = statement.size();
    for (sg_i = sg_operatorIndex + 2; sg_i < sg_statementSize; ++sg_i)
    {
        if (std::none_of(std::begin(scg_validNumerics),
                         std::end(scg_validNumerics),
                         [&](const char &c) noexcept -> const bool {
                             return c == statement[sg_i];
                         }))
            break;
        if (statement[sg_i] == '.')
            return false;
    }
    return true;
}
// Checks if left operand is int or float
// Returns true if int false if float
static bool isLOIntOrFloat(const std::string &statement)
{
    bool isInt{true};
    for (sg_i = sg_operatorIndex - 2; sg_i >= 0; --sg_i)
    {
        if (std::none_of(std::begin(scg_validNumerics),
                         std::end(scg_validNumerics),
                         [&](const char &c) noexcept -> const bool {
                             return c == statement[sg_i];
                         }))
            break;
        if (statement[sg_i] == '.')
            isInt = false;
    }
    sg_leftOperatorStartIndex = sg_i;
    return isInt;
}

// Evaluates operator+
static void evalopplus(std::string &statement, const int &line, const char *&filename)
{
    // string + _
    if (statement[sg_operatorIndex - 2] == '"')
    {
        std::string leftOperand;
        getLeftOperand(statement, leftOperand);
        // string + string
        if (statement[sg_operatorIndex + 2] == '"')
        {
            std::string rightOperand;
            getRightOperand(statement, rightOperand);
            statement.replace(sg_leftOperatorStartIndex + 1, leftOperand.size() + rightOperand.size() + 5,
                              leftOperand + rightOperand);
        }
        // string + int
        else if (isROIntOrFloat(statement))
        {
            int rightOperand;
            getRightOperand(statement, rightOperand);
            statement.replace(sg_leftOperatorStartIndex + 1,
                              leftOperand.size() + std::to_string(rightOperand).size() + 4,
                              leftOperand + std::to_string(rightOperand) + '"');
        }
        // string + float
        else
        {
            float rightOperand;
            getRightOperand(statement, rightOperand);
            statement.replace(sg_leftOperatorStartIndex + 1, leftOperand.size() + sg_i - sg_operatorIndex + 2,
                              leftOperand + std::to_string(rightOperand) + '"');
        }
    }
    // int + _
    else if (isLOIntOrFloat(statement))
    {
        int leftOperand;
        getLeftOperand(statement, leftOperand);
        // int + string
        if (statement[sg_operatorIndex + 2] == '"')
        {
            std::string rightOperand;
            getRightOperand(statement, rightOperand);
            statement.replace(sg_leftOperatorStartIndex + 1,
                              std::to_string(leftOperand).size() + rightOperand.size() + 5,
                              '"' + std::to_string(leftOperand) + rightOperand + '"');
        }
        // int + int
        else if (isROIntOrFloat(statement))
        {
            int rightOperand;
            getRightOperand(statement, rightOperand);
            statement.replace(sg_leftOperatorStartIndex + 1,
                              std::to_string(leftOperand).size() + std::to_string(rightOperand).size() + 3,
                              std::to_string(leftOperand + rightOperand));
        }
        // int + float
        else
        {
            float rightOperand;
            getRightOperand(statement, rightOperand);
            statement.replace(sg_leftOperatorStartIndex + 1,
                              std::to_string(leftOperand).size() + sg_i - sg_operatorIndex + 2,
                              std::to_string(leftOperand + rightOperand));
        }
    }
    // float + _
    else
    {
        float leftOperand;
        getLeftOperand(statement, leftOperand);
        // float + string
        if (statement[sg_operatorIndex + 2] == '"')
        {
            std::string rightOperand;
            getRightOperand(statement, rightOperand);
            statement.replace(sg_leftOperatorStartIndex + 1,
                              sg_operatorIndex - sg_leftOperatorStartIndex + rightOperand.size() + 5,
                              '"' + std::to_string(leftOperand) + rightOperand + '"');
        }
        // float + int
        else if (isROIntOrFloat(statement))
        {
            int rightOperand;
            getRightOperand(statement, rightOperand);
            statement.replace(sg_leftOperatorStartIndex + 1,
                              sg_operatorIndex - sg_leftOperatorStartIndex + std::to_string(rightOperand).size() + 3,
                              std::to_string(leftOperand + rightOperand));
        }
        else
            teaSyntaxError(line, filename, "Invalid right operand for operator+.");
    }
}

// Evaluates and operators in a given string
// Returns modified string with value(s) in-place
std::string evalOps(std::string statement, const int &line, const char *&filename)
{
    // Operator+
    while (true)
    {
        sg_operatorIndex = searchOperator(statement, "+");
        if (sg_operatorIndex != statement.npos)
            evalopplus(statement, line, filename);
        else
            break;
    }
    return statement;
}
