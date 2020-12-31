#include "error.hpp"
#include "k_assign.hpp"

// Defining static globals (hence the sg_ prefix)
// These are defined here for performance reasons
static std::string sg_assignmentType;
static int sg_i;
static int sg_statementSize;
static std::string sg_var;
static std::string sg_varname;

// Gets assignment type
static void getAssignmentType(const std::string &statement, const int &line, const char *&filename)
{
    sg_assignmentType.clear();
    for (sg_i = 7; sg_i < sg_statementSize; ++sg_i)
    {
        if (statement[sg_i] == ' ')
            break;
        sg_assignmentType.push_back(statement[sg_i]);
    }
    if (sg_assignmentType == "")
        teaSyntaxError(line, filename, "Assignment type not given.");
}

// Gets var name
static void getVarname(const std::string &statement, const int &line, const char *&filename)
{
    sg_varname.clear();
    for (++sg_i; sg_i < sg_statementSize; ++sg_i)
    {
        if (statement[sg_i] == ' ')
            break;
        sg_varname.push_back(statement[sg_i]);
    }
    if (sg_varname == "")
        teaSyntaxError(line, filename, "Variable name not given.");
}

// Gets var
static void getVar(const std::string &statement, const int &line, const char *&filename)
{
    sg_var.clear();
    for (++sg_i; sg_i < sg_statementSize; ++sg_i)
        sg_var.push_back(statement[sg_i]);
    if (sg_var == "")
        teaSyntaxError(line, filename, "New variable not given.");
}

// Varifies new string variable
static void verifyStringVar(const int &line, const char *&filename)
{
    if (sg_var[0] != '"' || sg_var[sg_var.size() - 1] != '"')
        teaSyntaxError(line, filename, "Invalid string.");
    sg_var.erase(0, 1);
    sg_var.pop_back();
}

// Changes string variable
static void changeStringVariable(teaString_t &teaStrings, const int &line, const char *&filename)
{
    for (TeaString &teaString : teaStrings)
    {
        if (teaString.getname() == sg_varname)
        {
            verifyStringVar(line, filename);
            teaString.setvalue(sg_var);
            return;
        }
    }
    teaSyntaxError(line, filename, "Variable name was not found.");
}

// Changes int variable
static void changeIntVariable(teaInt_t &teaInts, const int &line, const char *&filename)
{
    for (TeaInt &teaInt : teaInts)
    {
        if (teaInt.getname() == sg_varname)
        {
            teaInt.setvalue(std::stoi(sg_var));
            return;
        }
    }
    teaSyntaxError(line, filename, "Variable name was not found.");
}

// Changes float variable
static void changeFloatVariable(teaFloat_t &teaFloats, const int &line, const char *&filename)
{
    for (TeaFloat &teaFloat : teaFloats)
    {
        if (teaFloat.getname() == sg_varname)
        {
            teaFloat.setvalue(std::stof(sg_var));
            return;
        }
    }
    teaSyntaxError(line, filename, "Variable name was not found.");
}

// Called when the assign keyword is called in tea
void kAssign(const std::string &statement, const int &line, const char *&filename, teaString_t &teaStrings,
             teaInt_t &teaInts, teaFloat_t &teaFloats)
{
    sg_statementSize = statement.size();
    getAssignmentType(statement, line, filename);
    getVarname(statement, line, filename);
    getVar(statement, line, filename);
    if (sg_assignmentType == "string")
        changeStringVariable(teaStrings, line, filename);
    else if (sg_assignmentType == "int")
        changeIntVariable(teaInts, line, filename);
    else if (sg_assignmentType == "float")
        changeFloatVariable(teaFloats, line, filename);
    else
        teaSyntaxError(line, filename, "Invalid assignment type.");
}
