#include "error.hpp"
#include "k_assign.hpp"
#include "runtea.hpp"

// Defining static globals (hence the sg_ prefix)
// These are defined here for performance reasons
static std::string sg_assignmentType;
static std::size_t sg_i;
static std::string sg_var;
static std::string sg_varname;

// Gets assignment type
static void getAssignmentType(const std::string &statement, const std::size_t &statementSize, const int &line,
    const char *&filename)
{
    sg_assignmentType.clear();
    for (sg_i = 7U; sg_i < statementSize; ++sg_i)
    {
        if (statement[sg_i] == ' ')
            break;
        sg_assignmentType.push_back(statement[sg_i]);
    }
    if (sg_assignmentType == "")
        teaSyntaxError(line, filename, "Assignment type not given.");
}

// Gets var name
static void getVarname(const std::string &statement, const std::size_t &statementSize, const int &line,
    const char *&filename)
{
    sg_varname.clear();
    for (++sg_i; sg_i < statementSize; ++sg_i)
    {
        if (statement[sg_i] == ' ')
            break;
        sg_varname.push_back(statement[sg_i]);
    }
    if (sg_varname == "")
        teaSyntaxError(line, filename, "Variable name not given.");
}

// Gets var
static void getVar(const std::string &statement, const std::size_t &statementSize, const int &line,
    const char *&filename)
{
    sg_var.clear();
    for (++sg_i; sg_i < statementSize; ++sg_i)
        sg_var.push_back(statement[sg_i]);
    if (sg_var == "")
        teaSyntaxError(line, filename, "New variable not given.");
}

// Varifies new string variable
static void verifyStringVar(const int &line, const char *&filename)
{
    const std::size_t &&varSize{sg_var.size()};
    if (varSize < 2U || sg_var[0U] != '"' || sg_var[varSize - 1U] != '"')
        teaSyntaxError(line, filename, "Invalid string.");
    sg_var.erase(0U, 1U);
    sg_var.pop_back();
}

// Verifies numeric variable
static void verifyNumericVar(const int &line, const char *&filename)
{
    if (sg_var.find(' ') != sg_var.npos)
        teaSyntaxError(line, filename, "Invalid numeric type.");
}

// Verifies array index and returns it
static const int verifyArrayIndex(const std::size_t &arrayNameSize, const int &line, const char *&filename)
{
    static std::string s_indexstr;
    s_indexstr.clear();
    if (sg_varname[arrayNameSize] != '[' || sg_varname.back() != ']')
        teaSyntaxError(line, filename, "Invalid array subscript.");
    const std::size_t &&varnameSize{sg_varname.size()};
    for (std::size_t &&i{arrayNameSize + 1U}; i < varnameSize; ++i)
        s_indexstr.push_back(sg_varname[i]);
    s_indexstr.pop_back();
    const int &&index{std::stoi(s_indexstr)};
    if (s_indexstr != std::to_string(index))
        teaSyntaxError(line, filename, "Invalid array subscript.");
    return index;
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
            verifyNumericVar(line, filename);
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
            verifyNumericVar(line, filename);
            teaFloat.setvalue(std::stof(sg_var));
            return;
        }
    }
    teaSyntaxError(line, filename, "Variable name was not found.");
}

// Changes array variable
static void changeArrayVariable(teaArray_t &teaArrays, const int &line, const char *&filename)
{
    for (TeaArray &teaArray : teaArrays)
    {
        if (startsWithKeyword(sg_varname, teaArray.getname().c_str()))
        {
            const int &&index{verifyArrayIndex(teaArray.getname().size(), line, filename)};
            if (index < 0 || index > static_cast<int>(teaArray.getsize() - 1U))
                teaSyntaxError(line, filename, "Invalid array index.");
            const std::string &arrayType{teaArray.gettype()};
            const std::any &any{teaArray.getdata()[index]};
            if (arrayType == "string")
            {
                const std::string &&var{sg_var.substr(1U, sg_var.size() - 2U)};
                const_cast<std::string &>(std::any_cast<const TeaString &>(any).getvalue()) = var;
            }
            else if (arrayType == "int")
                const_cast<int &>(std::any_cast<const TeaInt &>(any).getvalue()) = std::stoi(sg_var);
            else if (arrayType == "float")
                const_cast<float &>(std::any_cast<const TeaFloat &>(any).getvalue()) = std::stof(sg_var);
            else
                teaSyntaxError(line, filename, "Invalid array type.");
            return;
        }
    }
    teaSyntaxError(line, filename, "Variable name was not found.");
}

// Called when the assign keyword is called in tea
void kAssign(const std::string &statement, const int &line, const char *&filename, teaString_t &teaStrings,
    teaInt_t &teaInts, teaFloat_t &teaFloats, teaArray_t &teaArrays)
{
    const std::size_t &&statementSize{statement.size()};
    getAssignmentType(statement, statementSize, line, filename);
    getVarname(statement, statementSize, line, filename);
    getVar(statement, statementSize, line, filename);
    if (sg_assignmentType == "string")
        changeStringVariable(teaStrings, line, filename);
    else if (sg_assignmentType == "int")
        changeIntVariable(teaInts, line, filename);
    else if (sg_assignmentType == "float")
        changeFloatVariable(teaFloats, line, filename);
    else if (sg_assignmentType == "array")
        changeArrayVariable(teaArrays, line, filename);
    else
        teaSyntaxError(line, filename, "Invalid assignment type.");
}
