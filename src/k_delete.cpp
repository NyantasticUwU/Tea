#include "error.hpp"
#include "k_delete.hpp"
#include "TeaString.hpp"
#include <algorithm>

// Defining static globals (hence the sg_ prefix)
// These are defined here for performance reasons
static int sg_i;
static std::string sg_name;
static teaString_t::iterator sg_nameIndex;
static int sg_statementSize;
static std::string sg_type;

// Gets variable type
static void getType(const std::string &statement)
{
    sg_type.clear();
    for (sg_i = 7; sg_i < sg_statementSize; ++sg_i)
    {
        if (statement[sg_i] == ' ')
        {
            ++sg_i;
            break;
        }
        sg_type.push_back(statement[sg_i]);
    }
}

// Gets variable name
static void getName(const std::string &statement)
{
    sg_name.clear();
    for (/* sg_i is first index */; sg_i < sg_statementSize; ++sg_i)
        sg_name.push_back(statement[sg_i]);
}

// Called when the delete keyword is called in tea
void kDelete(const std::string &statement, const int &line, const char *&filename, teaString_t &teaStrings)
{
    if (std::count(statement.begin(), statement.end(), ' ') != 2)
        teaSyntaxError(line, filename);
    sg_statementSize = statement.size();
    getType(statement);
    getName(statement);
    if (sg_type == "string")
    {
        sg_nameIndex = std::find_if(teaStrings.begin(), teaStrings.end(),
                                    [&](const TeaString &ts) -> const bool {
                                        return ts.getname() == sg_name;
                                    });
        if (sg_nameIndex == teaStrings.end())
            teaSyntaxError(line, filename, "Variable name not found.");
        teaStrings.erase(sg_nameIndex);
    }
    else
        teaSyntaxError(line, filename, "Invalid type specifier.");
}
