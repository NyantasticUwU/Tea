#include "constants.hpp"
#include "error.hpp"
#include "evalops.hpp"
#include <algorithm>
#include <cmath>
#include <vector>

// Defining static globals (hence the sg_ prefix)
// These are defined here for performance reasons
static const std::vector<std::string> scg_ops[TEA_NUMBER_OF_OPERATOR_GROUPS]{
    {"("},
    {" ^ "},
    {" * ", " / ", " % "},
    {" + ", " - "},
    {" == ", " != ", " < ", " > ", " <= ", " >= "},
    {" && ", " || "}};
static constexpr char scg_validNumerics[15]{"0123456789.xX-"};
static int sg_i;
static int sg_leftOperatorStartIndex; // Indicates start of left operand
static int sg_operatorIndex;

// Searches for operator
static int searchOperator(const std::string &statement, const std::string &op, const bool &isSpaceBefore)
{
    bool &&isInString{false};
    const std::size_t &&statementSize{statement.size()};
    for (std::size_t &&i{0U}; i < statementSize; ++i)
    {
        if (statement[i] == '"') // statement[i - 1] will not be a backslash
        {
            isInString = isInString ? false : true;
            if (isInString)
                sg_leftOperatorStartIndex = i;
            continue;
        }
        if (statement[i] == '\\' && (statement[i + 1U] == '\\' || statement[i + 1U] == '"'))
        {
            ++i;
            continue;
        }
        if (!isInString && statement.substr(i > 0U && isSpaceBefore ? i - 1U : i, op.size()) == op)
            return i;
    }
    return statement.npos;
}

// Gets left operand
static std::string getLeftOperand(const std::string &statement, const std::string &)
{
    std::string leftOperand;
    for (sg_i = sg_leftOperatorStartIndex + 1; sg_i < sg_operatorIndex - 2; ++sg_i)
        leftOperand.push_back(statement[sg_i]);
    return leftOperand;
}
// Gets left operand
static int getLeftOperand(const std::string &statement, const int &)
{
    std::string intstr;
    for (sg_i = sg_leftOperatorStartIndex + 1; sg_i < sg_operatorIndex - 1; ++sg_i)
        intstr.push_back(statement[sg_i]);
    return std::stoi(intstr);
}
// Gets left operand
static float getLeftOperand(const std::string &statement, const float &)
{
    std::string floatstr;
    for (sg_i = sg_leftOperatorStartIndex + 1; sg_i < sg_operatorIndex - 1; ++sg_i)
        floatstr.push_back(statement[sg_i]);
    return std::stof(floatstr);
}

// Gets right operand
static std::string getRightOperand(const std::string &statement, const int &opsize, const std::string &)
{
    std::string rightOperand;
    const int &&statementSize{static_cast<int>(statement.size())};
    for (sg_i = sg_operatorIndex + opsize + 2; sg_i < statementSize; ++sg_i)
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
    return rightOperand;
}
// Gets right operand
static int getRightOperand(const std::string &statement, const int &opsize, const int &)
{
    int &&statementSize{static_cast<int>(statement.size())};
    std::string intstr;
    for (sg_i = sg_operatorIndex + opsize + 1; sg_i < statementSize; ++sg_i)
    {
        if (std::none_of(std::begin(scg_validNumerics),
                         std::end(scg_validNumerics),
                         [&](const char &c) noexcept -> const bool {
                             return c == statement[sg_i];
                         }))
            break;
        intstr.push_back(statement[sg_i]);
    }
    return std::stoi(intstr, nullptr, 0);
}
// Gets right operand
static float getRightOperand(const std::string &statement, const int &opsize, const float &)
{
    int &&statementSize{static_cast<int>(statement.size())};
    std::string floatstr;
    for (sg_i = sg_operatorIndex + opsize + 1; sg_i < statementSize; ++sg_i)
    {
        if (std::none_of(std::begin(scg_validNumerics),
                         std::end(scg_validNumerics),
                         [&](const char &c) noexcept -> const bool {
                             return c == statement[sg_i];
                         }))
            break;
        floatstr.push_back(statement[sg_i]);
    }
    return std::stof(floatstr);
}

// Checks if right operand is int or float
// Returns true if int false if float
static bool isROIntOrFloat(const std::string &statement, const int &opsize)
{
    int &&statementSize{static_cast<int>(statement.size())};
    for (sg_i = sg_operatorIndex + opsize + 1; sg_i < statementSize; ++sg_i)
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
    static bool isInt;
    isInt = true;
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

// Evaluates operator()
static void evalopbrace(std::string &statement, const int &line, const char *&filename)
{
    bool &&isInString{false};
    int &&statementSize{static_cast<int>(statement.size())};
    int &&opar{0};
    std::string evalstatement;
    for (sg_i = sg_operatorIndex + 1; sg_i < statementSize; ++sg_i)
    {
        if (statement[sg_i] == '(' && !isInString)
        {
            ++opar;
            evalstatement.push_back('(');
            continue;
        }
        if (statement[sg_i] == ')' && !isInString)
        {
            if (!opar--)
                break;
            evalstatement.push_back(')');
            continue;
        }
        if (statement[sg_i] == '"') // statement[sg_i - 1] will not be a backslash
        {
            isInString = isInString ? false : true;
            evalstatement.push_back('"');
            continue;
        }
        if (statement[sg_i] == '\\' && (statement[sg_i + 1] == '\\' || statement[sg_i + 1] == '"'))
        {
            evalstatement.push_back(statement[sg_i]);
            evalstatement.push_back(statement[++sg_i]);
            continue;
        }
        evalstatement.push_back(statement[sg_i]);
    }
    if (!opar)
        teaSyntaxError(line, filename, "Operator() is never closed.");
    const int oi{sg_operatorIndex};
    const int &&i{sg_i - sg_operatorIndex + 1};
    evalOps(evalstatement, line, filename);
    statement.replace(oi, i, evalstatement);
}

// Evaluates operator^
static void evalopex(std::string &statement)
{
    // int ^ _
    if (isLOIntOrFloat(statement))
    {
        const int leftOperand{getLeftOperand(statement, 0)};
        // int ^ int
        if (isROIntOrFloat(statement, 1))
        {
            const int rightOperand{getRightOperand(statement, 1, 0)};
            statement.replace(sg_leftOperatorStartIndex + 1,
                              std::to_string(leftOperand).size() + std::to_string(rightOperand).size() + 3,
                              std::to_string(static_cast<int>(std::pow(leftOperand, rightOperand))));
        }
        // int ^ float
        else
        {
            const float rightOperand{getRightOperand(statement, 1, 0.0f)};
            statement.replace(sg_leftOperatorStartIndex + 1,
                              std::to_string(leftOperand).size() + sg_i - sg_operatorIndex + 1,
                              std::to_string(static_cast<float>(std::pow(leftOperand, rightOperand))));
        }
    }
    // float ^ _
    else
    {
        const float leftOperand{getLeftOperand(statement, 0.0f)};
        // float ^ int
        if (isROIntOrFloat(statement, 1))
        {
            const int rightOperand{getRightOperand(statement, 1, 0)};
            statement.replace(sg_leftOperatorStartIndex + 1,
                              sg_operatorIndex - sg_leftOperatorStartIndex +
                                  std::to_string(rightOperand).size() + 1,
                              std::to_string(static_cast<float>(std::pow(leftOperand, rightOperand))));
        }
        // float ^ float
        else
        {
            const float rightOperand{getRightOperand(statement, 1, 0.0f)};
            statement.replace(sg_leftOperatorStartIndex + 1,
                              sg_operatorIndex - sg_leftOperatorStartIndex +
                                  sg_i - sg_operatorIndex - 1,
                              std::to_string(std::pow(leftOperand, rightOperand)));
        }
    }
}

// Evaluates operator*
static void evalopasterisk(std::string &statement)
{
    // int * _
    if (isLOIntOrFloat(statement))
    {
        const int leftOperand{getLeftOperand(statement, 0)};
        // int * int
        if (isROIntOrFloat(statement, 1))
        {
            const int rightOperand{getRightOperand(statement, 1, 0)};
            statement.replace(sg_leftOperatorStartIndex + 1,
                              std::to_string(leftOperand).size() + std::to_string(rightOperand).size() + 3,
                              std::to_string(leftOperand * rightOperand));
        }
        // int * float
        else
        {
            const float rightOperand{getRightOperand(statement, 1, 0.0f)};
            statement.replace(sg_leftOperatorStartIndex + 1,
                              std::to_string(leftOperand).size() + sg_i - sg_operatorIndex + 1,
                              std::to_string(leftOperand * rightOperand));
        }
    }
    // float * _
    else
    {
        const float leftOperand{getLeftOperand(statement, 0.0f)};
        // float * int
        if (isROIntOrFloat(statement, 1))
        {
            const int rightOperand{getRightOperand(statement, 1, 0)};
            statement.replace(sg_leftOperatorStartIndex + 1,
                              sg_operatorIndex - sg_leftOperatorStartIndex +
                                  std::to_string(rightOperand).size() + 1,
                              std::to_string(leftOperand * rightOperand));
        }
        // float * float
        else
        {
            const float rightOperand{getRightOperand(statement, 1, 0.0f)};
            statement.replace(sg_leftOperatorStartIndex + 1,
                              sg_operatorIndex - sg_leftOperatorStartIndex +
                                  sg_i - sg_operatorIndex - 1,
                              std::to_string(leftOperand * rightOperand));
        }
    }
}

// Evaluates operator/
static void evalopfwslash(std::string &statement)
{
    // int / _
    if (isLOIntOrFloat(statement))
    {
        const int leftOperand{getLeftOperand(statement, 0)};
        // int / int
        if (isROIntOrFloat(statement, 1))
        {
            const int rightOperand{getRightOperand(statement, 1, 0)};
            statement.replace(sg_leftOperatorStartIndex + 1,
                              std::to_string(leftOperand).size() + std::to_string(rightOperand).size() + 3,
                              std::to_string(leftOperand / rightOperand));
        }
        // int / float
        else
        {
            const float rightOperand{getRightOperand(statement, 1, 0.0f)};
            statement.replace(sg_leftOperatorStartIndex + 1,
                              std::to_string(leftOperand).size() + sg_i - sg_operatorIndex + 1,
                              std::to_string(leftOperand / rightOperand));
        }
    }
    // float / _
    else
    {
        const float leftOperand{getLeftOperand(statement, 0.0f)};
        // float / int
        if (isROIntOrFloat(statement, 1))
        {
            const int rightOperand{getRightOperand(statement, 1, 0)};
            statement.replace(sg_leftOperatorStartIndex + 1,
                              sg_operatorIndex - sg_leftOperatorStartIndex +
                                  std::to_string(rightOperand).size() + 1,
                              std::to_string(leftOperand / rightOperand));
        }
        // float / float
        else
        {
            const float rightOperand{getRightOperand(statement, 1, 0.0f)};
            statement.replace(sg_leftOperatorStartIndex + 1,
                              sg_operatorIndex - sg_leftOperatorStartIndex +
                                  sg_i - sg_operatorIndex - 1,
                              std::to_string(leftOperand / rightOperand));
        }
    }
}

// Evaluates operator%
static void evalopmod(std::string &statement, const int &line, const char *&filename)
{
    // int % _
    if (isLOIntOrFloat(statement))
    {
        const int leftOperand{getLeftOperand(statement, 0)};
        // int % int
        if (isROIntOrFloat(statement, 1))
        {
            const int rightOperand{getRightOperand(statement, 1, 0)};
            statement.replace(sg_leftOperatorStartIndex + 1,
                              std::to_string(leftOperand).size() + std::to_string(rightOperand).size() + 3,
                              std::to_string(leftOperand % rightOperand));
        }
        else
            teaSyntaxError(line, filename, "Invalid right operand for operator%.");
    }
    else
        teaSyntaxError(line, filename, "Invalid left operand for operator%.");
}

// Evaluates operator+
static void evalopplus(std::string &statement)
{
    // string + _
    if (statement[sg_operatorIndex - 2] == '"')
    {
        const std::string leftOperand{getLeftOperand(statement, "")};
        // string + string
        if (statement[sg_operatorIndex + 2] == '"')
        {
            const std::string rightOperand{getRightOperand(statement, 1, "")};
            statement.replace(sg_leftOperatorStartIndex + 1, leftOperand.size() + rightOperand.size() + 5,
                              leftOperand + rightOperand);
        }
        // string + int
        else if (isROIntOrFloat(statement, 1))
        {
            const int rightOperand{getRightOperand(statement, 1, 0)};
            statement.replace(sg_leftOperatorStartIndex + 1,
                              leftOperand.size() + std::to_string(rightOperand).size() + 4,
                              leftOperand + std::to_string(rightOperand) + '"');
        }
        // string + float
        else
        {
            const float rightOperand{getRightOperand(statement, 1, 0.0f)};
            statement.replace(sg_leftOperatorStartIndex + 1, leftOperand.size() + sg_i - sg_operatorIndex + 2,
                              leftOperand + std::to_string(rightOperand) + '"');
        }
    }
    // int + _
    else if (isLOIntOrFloat(statement))
    {
        const int leftOperand{getLeftOperand(statement, 0)};
        // int + string
        if (statement[sg_operatorIndex + 2] == '"')
        {
            const std::string rightOperand{getRightOperand(statement, 1, "")};
            statement.replace(sg_leftOperatorStartIndex + 1,
                              std::to_string(leftOperand).size() + rightOperand.size() + 5,
                              '"' + std::to_string(leftOperand) + rightOperand + '"');
        }
        // int + int
        else if (isROIntOrFloat(statement, 1))
        {
            const int rightOperand{getRightOperand(statement, 1, 0)};
            statement.replace(sg_leftOperatorStartIndex + 1,
                              std::to_string(leftOperand).size() + std::to_string(rightOperand).size() + 3,
                              std::to_string(leftOperand + rightOperand));
        }
        // int + float
        else
        {
            const float rightOperand{getRightOperand(statement, 1, 0.0f)};
            statement.replace(sg_leftOperatorStartIndex + 1,
                              std::to_string(leftOperand).size() + sg_i - sg_operatorIndex + 1,
                              std::to_string(leftOperand + rightOperand));
        }
    }
    // float + _
    else
    {
        const float leftOperand{getLeftOperand(statement, 0.0f)};
        // float + string
        if (statement[sg_operatorIndex + 2] == '"')
        {
            const std::string rightOperand{getRightOperand(statement, 1, "")};
            statement.replace(sg_leftOperatorStartIndex + 1,
                              sg_operatorIndex - sg_leftOperatorStartIndex + rightOperand.size() + 3,
                              '"' + std::to_string(leftOperand) + rightOperand + '"');
        }
        // float + int
        else if (isROIntOrFloat(statement, 1))
        {
            const int rightOperand{getRightOperand(statement, 1, 0)};
            statement.replace(sg_leftOperatorStartIndex + 1,
                              sg_operatorIndex - sg_leftOperatorStartIndex +
                                  std::to_string(rightOperand).size() + 1,
                              std::to_string(leftOperand + rightOperand));
        }
        // float + float
        else
        {
            const float rightOperand{getRightOperand(statement, 1, 0.0f)};
            statement.replace(sg_leftOperatorStartIndex + 1,
                              sg_operatorIndex - sg_leftOperatorStartIndex +
                                  sg_i - sg_operatorIndex - 1,
                              std::to_string(leftOperand + rightOperand));
        }
    }
}

// Evaluates operator-
static void evalopminus(std::string &statement)
{
    // int - _
    if (isLOIntOrFloat(statement))
    {
        const int leftOperand{getLeftOperand(statement, 0)};
        // int - int
        if (isROIntOrFloat(statement, 1))
        {
            const int rightOperand{getRightOperand(statement, 1, 0)};
            statement.replace(sg_leftOperatorStartIndex + 1,
                              std::to_string(leftOperand).size() + std::to_string(rightOperand).size() + 3,
                              std::to_string(leftOperand - rightOperand));
        }
        // int - float
        else
        {
            const float rightOperand{getRightOperand(statement, 1, 0.0f)};
            statement.replace(sg_leftOperatorStartIndex + 1,
                              std::to_string(leftOperand).size() + sg_i - sg_operatorIndex + 1,
                              std::to_string(leftOperand - rightOperand));
        }
    }
    // float - _
    else
    {
        const float leftOperand{getLeftOperand(statement, 0.0f)};
        // float - int
        if (isROIntOrFloat(statement, 1))
        {
            const int rightOperand{getRightOperand(statement, 1, 0)};
            statement.replace(sg_leftOperatorStartIndex + 1,
                              sg_operatorIndex - sg_leftOperatorStartIndex +
                                  std::to_string(rightOperand).size() + 1,
                              std::to_string(leftOperand - rightOperand));
        }
        // float - float
        else
        {
            const float rightOperand{getRightOperand(statement, 1, 0.0f)};
            statement.replace(sg_leftOperatorStartIndex + 1,
                              sg_operatorIndex - sg_leftOperatorStartIndex +
                                  sg_i - sg_operatorIndex - 1,
                              std::to_string(leftOperand - rightOperand));
        }
    }
}

// Evaluates operator==
static void evalopequal(std::string &statement, const int &line, const char *&filename)
{
    // string == _
    if (statement[sg_operatorIndex - 2] == '"')
    {
        const std::string leftOperand{getLeftOperand(statement, "")};
        // string == string
        if (statement[sg_operatorIndex + 3] == '"')
        {
            const std::string rightOperand{getRightOperand(statement, 2, "")};
            statement.replace(sg_leftOperatorStartIndex, leftOperand.size() + rightOperand.size() + 8,
                              leftOperand == rightOperand ? "1" : "0");
        }
        else
            teaSyntaxError(line, filename, "Invalid right operand for operator==.");
    }
    // int == _
    else if (isLOIntOrFloat(statement))
    {
        const int leftOperand{getLeftOperand(statement, 0)};
        // int == int
        if (isROIntOrFloat(statement, 2))
        {
            const int rightOperand{getRightOperand(statement, 2, 0)};
            statement.replace(sg_leftOperatorStartIndex + 1,
                              std::to_string(leftOperand).size() + std::to_string(rightOperand).size() + 4,
                              leftOperand == rightOperand ? "1" : "0");
        }
        else
            teaSyntaxError(line, filename, "Invalid right operand for operator==.");
    }
    else
        teaSyntaxError(line, filename, "Invalid left operand for operator==.");
}

// Evaluates operator!=
static void evalopnotequal(std::string &statement, const int &line, const char *&filename)
{
    // string != _
    if (statement[sg_operatorIndex - 2] == '"')
    {
        const std::string leftOperand{getLeftOperand(statement, "")};
        // string != string
        if (statement[sg_operatorIndex + 3] == '"')
        {
            const std::string rightOperand{getRightOperand(statement, 2, "")};
            statement.replace(sg_leftOperatorStartIndex, leftOperand.size() + rightOperand.size() + 8,
                              leftOperand != rightOperand ? "1" : "0");
        }
        else
            teaSyntaxError(line, filename, "Invalid right operand for operator!=.");
    }
    // int != _
    else if (isLOIntOrFloat(statement))
    {
        const int leftOperand{getLeftOperand(statement, 0)};
        // int != int
        if (isROIntOrFloat(statement, 2))
        {
            const int rightOperand{getRightOperand(statement, 2, 0)};
            statement.replace(sg_leftOperatorStartIndex + 1,
                              std::to_string(leftOperand).size() + std::to_string(rightOperand).size() + 4,
                              leftOperand != rightOperand ? "1" : "0");
        }
        else
            teaSyntaxError(line, filename, "Invalid right operand for operator!=.");
    }
    else
        teaSyntaxError(line, filename, "Invalid left operand for operator!=.");
}

// Evaluates operator<
static void evalopless(std::string &statement, const int &line, const char *&filename)
{
    // string < _
    if (statement[sg_operatorIndex - 2] == '"')
    {
        const std::string leftOperand{getLeftOperand(statement, "")};
        // string < string
        if (statement[sg_operatorIndex + 2] == '"')
        {
            const std::string rightOperand{getRightOperand(statement, 1, "")};
            statement.replace(sg_leftOperatorStartIndex, leftOperand.size() + rightOperand.size() + 7,
                              leftOperand < rightOperand ? "1" : "0");
        }
        else
            teaSyntaxError(line, filename, "Invalid right operand for operator<.");
    }
    // int < _
    else if (isLOIntOrFloat(statement))
    {
        const int leftOperand{getLeftOperand(statement, 0)};
        // int < int
        if (isROIntOrFloat(statement, 1))
        {
            const int rightOperand{getRightOperand(statement, 1, 0)};
            statement.replace(sg_leftOperatorStartIndex + 1,
                              std::to_string(leftOperand).size() + std::to_string(rightOperand).size() + 3,
                              leftOperand < rightOperand ? "1" : "0");
        }
        // int < float
        else
        {
            const float rightOperand{getRightOperand(statement, 1, 0.0f)};
            statement.replace(sg_leftOperatorStartIndex + 1,
                              std::to_string(leftOperand).size() + sg_i - sg_operatorIndex + 1,
                              leftOperand < rightOperand ? "1" : "0");
        }
    }
    // float < _
    else
    {
        const float leftOperand{getLeftOperand(statement, 0.0f)};
        // float < int
        if (isROIntOrFloat(statement, 1))
        {
            const int rightOperand{getRightOperand(statement, 1, 0)};
            statement.replace(sg_leftOperatorStartIndex + 1,
                              sg_operatorIndex - sg_leftOperatorStartIndex +
                                  std::to_string(rightOperand).size() + 1,
                              leftOperand < rightOperand ? "1" : "0");
        }
        // float < float
        else
        {
            const float rightOperand{getRightOperand(statement, 1, 0.0f)};
            statement.replace(sg_leftOperatorStartIndex + 1,
                              sg_operatorIndex - sg_leftOperatorStartIndex +
                                  sg_i - sg_operatorIndex - 1,
                              leftOperand < rightOperand ? "1" : "0");
        }
    }
}

// Evaluates operator>
static void evalopgreater(std::string &statement, const int &line, const char *&filename)
{
    // string > _
    if (statement[sg_operatorIndex - 2] == '"')
    {
        const std::string leftOperand{getLeftOperand(statement, "")};
        // string > string
        if (statement[sg_operatorIndex + 2] == '"')
        {
            const std::string rightOperand{getRightOperand(statement, 1, "")};
            statement.replace(sg_leftOperatorStartIndex, leftOperand.size() + rightOperand.size() + 7,
                              leftOperand > rightOperand ? "1" : "0");
        }
        else
            teaSyntaxError(line, filename, "Invalid right operand for operator>.");
    }
    // int > _
    else if (isLOIntOrFloat(statement))
    {
        const int leftOperand{getLeftOperand(statement, 0)};
        // int > int
        if (isROIntOrFloat(statement, 1))
        {
            const int rightOperand{getRightOperand(statement, 1, 0)};
            statement.replace(sg_leftOperatorStartIndex + 1,
                              std::to_string(leftOperand).size() + std::to_string(rightOperand).size() + 3,
                              leftOperand > rightOperand ? "1" : "0");
        }
        // int > float
        else
        {
            const float rightOperand{getRightOperand(statement, 1, 0.0f)};
            statement.replace(sg_leftOperatorStartIndex + 1,
                              std::to_string(leftOperand).size() + sg_i - sg_operatorIndex + 1,
                              leftOperand > rightOperand ? "1" : "0");
        }
    }
    // float > _
    else
    {
        const float leftOperand{getLeftOperand(statement, 0.0f)};
        // float > int
        if (isROIntOrFloat(statement, 1))
        {
            const int rightOperand{getRightOperand(statement, 1, 0)};
            statement.replace(sg_leftOperatorStartIndex + 1,
                              sg_operatorIndex - sg_leftOperatorStartIndex +
                                  std::to_string(rightOperand).size() + 1,
                              leftOperand > rightOperand ? "1" : "0");
        }
        // float > float
        else
        {
            const float rightOperand{getRightOperand(statement, 1, 0.0f)};
            statement.replace(sg_leftOperatorStartIndex + 1,
                              sg_operatorIndex - sg_leftOperatorStartIndex +
                                  sg_i - sg_operatorIndex - 1,
                              leftOperand > rightOperand ? "1" : "0");
        }
    }
}

// Evaluates operator<=
static void evaloplessequal(std::string &statement, const int &line, const char *&filename)
{
    // string <= _
    if (statement[sg_operatorIndex - 2] == '"')
    {
        const std::string leftOperand{getLeftOperand(statement, "")};
        // string <= string
        if (statement[sg_operatorIndex + 3] == '"')
        {
            const std::string rightOperand{getRightOperand(statement, 2, "")};
            statement.replace(sg_leftOperatorStartIndex, leftOperand.size() + rightOperand.size() + 8,
                              leftOperand <= rightOperand ? "1" : "0");
        }
        else
            teaSyntaxError(line, filename, "Invalid right operand for operator<=.");
    }
    // int <= _
    else if (isLOIntOrFloat(statement))
    {
        const int leftOperand{getLeftOperand(statement, 0)};
        // int <= int
        if (isROIntOrFloat(statement, 2))
        {
            const int rightOperand{getRightOperand(statement, 2, 0)};
            statement.replace(sg_leftOperatorStartIndex + 1,
                              std::to_string(leftOperand).size() + std::to_string(rightOperand).size() + 4,
                              leftOperand <= rightOperand ? "1" : "0");
        }
        // int <= float
        else
        {
            const float rightOperand{getRightOperand(statement, 2, 0.0f)};
            statement.replace(sg_leftOperatorStartIndex + 1,
                              std::to_string(leftOperand).size() + sg_i - sg_operatorIndex + 2,
                              leftOperand <= rightOperand ? "1" : "0");
        }
    }
    // float <= _
    else
    {
        const float leftOperand{getLeftOperand(statement, 0.0f)};
        // float <= int
        if (isROIntOrFloat(statement, 2))
        {
            const int rightOperand{getRightOperand(statement, 2, 0)};
            statement.replace(sg_leftOperatorStartIndex + 1,
                              sg_operatorIndex - sg_leftOperatorStartIndex +
                                  std::to_string(rightOperand).size() + 2,
                              leftOperand <= rightOperand ? "1" : "0");
        }
        // float <= float
        else
        {
            const float rightOperand{getRightOperand(statement, 2, 0.0f)};
            statement.replace(sg_leftOperatorStartIndex + 1,
                              sg_operatorIndex - sg_leftOperatorStartIndex +
                                  sg_i - sg_operatorIndex,
                              leftOperand <= rightOperand ? "1" : "0");
        }
    }
}

// Evaluates operator&&
static void evalopand(std::string &statement)
{
    // int && _
    if (isLOIntOrFloat(statement))
    {
        const int leftOperand{getLeftOperand(statement, 0)};
        // int && int
        if (isROIntOrFloat(statement, 2))
        {
            const int rightOperand{getRightOperand(statement, 2, 0)};
            statement.replace(sg_leftOperatorStartIndex + 1,
                              std::to_string(leftOperand).size() + std::to_string(rightOperand).size() + 4,
                              leftOperand && rightOperand ? "1" : "0");
        }
        // int && float
        else
        {
            const float rightOperand{getRightOperand(statement, 2, 0.0f)};
            statement.replace(sg_leftOperatorStartIndex + 1,
                              std::to_string(leftOperand).size() + sg_i - sg_operatorIndex + 2,
                              leftOperand && rightOperand ? "1" : "0");
        }
    }
    // float && _
    else
    {
        const float leftOperand{getLeftOperand(statement, 0.0f)};
        // float && int
        if (isROIntOrFloat(statement, 2))
        {
            const int rightOperand{getRightOperand(statement, 2, 0)};
            statement.replace(sg_leftOperatorStartIndex + 1,
                              sg_operatorIndex - sg_leftOperatorStartIndex +
                                  std::to_string(rightOperand).size() + 2,
                              leftOperand && rightOperand ? "1" : "0");
        }
        // float && float
        else
        {
            const float rightOperand{getRightOperand(statement, 2, 0.0f)};
            statement.replace(sg_leftOperatorStartIndex + 1,
                              sg_operatorIndex - sg_leftOperatorStartIndex +
                                  sg_i - sg_operatorIndex,
                              leftOperand && rightOperand ? "1" : "0");
        }
    }
}

// Evaluates operator||
static void evalopor(std::string &statement)
{
    // int || _
    if (isLOIntOrFloat(statement))
    {
        const int leftOperand{getLeftOperand(statement, 0)};
        // int || int
        if (isROIntOrFloat(statement, 2))
        {
            const int rightOperand{getRightOperand(statement, 2, 0)};
            statement.replace(sg_leftOperatorStartIndex + 1,
                              std::to_string(leftOperand).size() + std::to_string(rightOperand).size() + 4,
                              leftOperand || rightOperand ? "1" : "0");
        }
        // int || float
        else
        {
            const float rightOperand{getRightOperand(statement, 2, 0.0f)};
            statement.replace(sg_leftOperatorStartIndex + 1,
                              std::to_string(leftOperand).size() + sg_i - sg_operatorIndex + 2,
                              leftOperand || rightOperand ? "1" : "0");
        }
    }
    // float || _
    else
    {
        const float leftOperand{getLeftOperand(statement, 0.0f)};
        // float || int
        if (isROIntOrFloat(statement, 2))
        {
            const int rightOperand{getRightOperand(statement, 2, 0)};
            statement.replace(sg_leftOperatorStartIndex + 1,
                              sg_operatorIndex - sg_leftOperatorStartIndex +
                                  std::to_string(rightOperand).size() + 2,
                              leftOperand || rightOperand ? "1" : "0");
        }
        // float || float
        else
        {
            const float rightOperand{getRightOperand(statement, 2, 0.0f)};
            statement.replace(sg_leftOperatorStartIndex + 1,
                              sg_operatorIndex - sg_leftOperatorStartIndex +
                                  sg_i - sg_operatorIndex,
                              leftOperand || rightOperand ? "1" : "0");
        }
    }
}

// Checks if sign is minus or negative
static bool checkSign(const char &c)
{
    if (c == '-')
        return std::any_of(std::begin(scg_validNumerics), std::end(scg_validNumerics),
                           [&](const char &ccc) noexcept -> const bool {
                               return *(&c + 1) == ccc;
                           })
                   ? false
                   : true;
    return true;
}

// Searches for operators by group
static std::string searchOperatorsByGroup(const std::string &statement)
{
    const std::size_t &&statementSize{statement.size()};
    for (const std::vector<std::string> &opg : scg_ops)
    {
        bool &&isInString{false};
        for (std::size_t &&i{0U}; i < statementSize; ++i)
        {
            const char &sc{statement[i]};
            if (sc == '"') // *(sc - 1) will not be a backslash
            {
                isInString = isInString ? false : true;
                continue;
            }
            if (sc == '\\' && (statement[i + 1U] == '\\' || statement[i + 1U] == '"'))
            {
                ++i;
                continue;
            }
            if (!isInString)
            {
                for (const std::string &c : opg)
                {
                    if (c == statement.substr(i, c.size()) && checkSign(sc))
                        return c;
                }
            }
        }
    }
    return "";
}

// Evaluates and operators in a given string
// Returns modified string with value(s) in-place
void evalOps(std::string &statement, const int &line, const char *&filename)
{
    while (true)
    {
        const std::string &&teaoperator{searchOperatorsByGroup(statement)};
        // No operator found
        if (teaoperator == "")
            break;
        // Operator()
        else if (teaoperator == "(")
        {
            sg_operatorIndex = searchOperator(statement, "(", false);
            evalopbrace(statement, line, filename);
            continue;
        }
        // Operator^
        else if (teaoperator == " ^ ")
        {
            sg_operatorIndex = searchOperator(statement, " ^ ", true);
            evalopex(statement);
            continue;
        }
        // Operator*
        else if (teaoperator == " * ")
        {
            sg_operatorIndex = searchOperator(statement, " * ", true);
            evalopasterisk(statement);
            continue;
        }
        // Operator/
        else if (teaoperator == " / ")
        {
            sg_operatorIndex = searchOperator(statement, " / ", true);
            evalopfwslash(statement);
            continue;
        }
        // Operator%
        else if (teaoperator == " % ")
        {
            sg_operatorIndex = searchOperator(statement, " % ", true);
            evalopmod(statement, line, filename);
            continue;
        }
        // Operator+
        else if (teaoperator == " + ")
        {
            sg_operatorIndex = searchOperator(statement, " + ", true);
            evalopplus(statement);
            continue;
        }
        // Operator-
        else if (teaoperator == " - ")
        {
            sg_operatorIndex = searchOperator(statement, " - ", true);
            evalopminus(statement);
            continue;
        }
        // operator==
        else if (teaoperator == " == ")
        {
            sg_operatorIndex = searchOperator(statement, " == ", true);
            evalopequal(statement, line, filename);
            continue;
        }
        // operator!=
        else if (teaoperator == " != ")
        {
            sg_operatorIndex = searchOperator(statement, " != ", true);
            evalopnotequal(statement, line, filename);
            continue;
        }
        // operator<
        else if (teaoperator == " < ")
        {
            sg_operatorIndex = searchOperator(statement, " < ", true);
            evalopless(statement, line, filename);
            continue;
        }
        // operator>
        else if (teaoperator == " > ")
        {
            sg_operatorIndex = searchOperator(statement, " > ", true);
            evalopgreater(statement, line, filename);
            continue;
        }
        // operator<=
        else if (teaoperator == " <= ")
        {
            sg_operatorIndex = searchOperator(statement, " <= ", true);
            evaloplessequal(statement, line, filename);
            continue;
        }
        // operator&&
        else if (teaoperator == " && ")
        {
            sg_operatorIndex = searchOperator(statement, " && ", true);
            evalopand(statement);
            continue;
        }
        // operator||
        else if (teaoperator == " || ")
        {
            sg_operatorIndex = searchOperator(statement, " || ", true);
            evalopor(statement);
            continue;
        }
    }
}
