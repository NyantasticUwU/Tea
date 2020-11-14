#include "error.hpp"
#include "evalops.hpp"
#include <algorithm>

// Defining static globals (hence the sg_ prefix)
// These are defined here for performance reasons
static constexpr char scg_validNumerics[14]{"0123456789.xX"};
static int sg_i;
static bool sg_isInString;
static int sg_leftStringStartIndex; // Used if left operand is a tea string (index of start of tea string)
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
                sg_leftStringStartIndex = sg_i;
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
    for (sg_i = sg_leftStringStartIndex + 1; sg_i < sg_operatorIndex - 2; ++sg_i)
        leftOperand.push_back(statement[sg_i]);
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
            statement.replace(sg_leftStringStartIndex + 1, leftOperand.size() + rightOperand.size() + 5,
                              leftOperand + rightOperand);
        }
        // string + int
        else if (isROIntOrFloat(statement))
        {
            int rightOperand;
            getRightOperand(statement, rightOperand);
            statement.replace(sg_leftStringStartIndex + 1,
                              leftOperand.size() + std::to_string(rightOperand).size() + 4,
                              leftOperand + std::to_string(rightOperand) + '"');
        }
        // string + float
        else
            teaSyntaxError(line, filename, "Invalid right operand for operator+.");
    }
    else
        teaSyntaxError(line, filename, "Invalid left operand for operator+.");
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
