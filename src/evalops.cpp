#include "error.hpp"
#include "evalops.hpp"

// Defining static globals (hence the sg_ prefix)
// These are defined here for performance reasons
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
        if (!sg_isInString && statement.substr(std::abs(sg_i - 1), op.size() + 2) == ' ' + op + ' ')
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

// Evaluates operator+
static void evalopplus(std::string &statement, const int &line, const char *&filename)
{
    if (statement[sg_operatorIndex - 2] == '"')
    {
        std::string leftOperand;
        getLeftOperand(statement, leftOperand);
        if (statement[sg_operatorIndex + 2] == '"')
        {
            std::string rightOperand;
            getRightOperand(statement, rightOperand);
            statement.replace(sg_leftStringStartIndex + 1, leftOperand.size() + rightOperand.size() + 5,
                              leftOperand + rightOperand);
        }
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
