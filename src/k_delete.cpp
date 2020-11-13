#include "error.hpp"
#include "k_delete.hpp"
#include <algorithm>

// Defining static globals (hence the sg_ prefix)
// These are defined here for performance reasons
static int sg_i;
static std::string sg_name;
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
void kDelete(const std::string &statement, const int &line, const char *&filename, teaString_t &teaStrings,
             teaInt_t &teaInts, teaFloat_t &teaFloats)
{
    if (std::count(statement.begin(), statement.end(), ' ') != 2)
        teaSyntaxError(line, filename);
    sg_statementSize = statement.size();
    getType(statement);
    getName(statement);
    if (sg_type == "float")
    {
        teaFloat_t::iterator nameIndex{std::find_if(teaFloats.begin(), teaFloats.end(),
                                                    [&](const TeaFloat &tf) noexcept -> const bool {
                                                        return tf.getname() == sg_name;
                                                    })};
        if (nameIndex == teaFloats.end())
            teaSyntaxError(line, filename, "Variable name not found.");
        teaFloats.erase(nameIndex);
    }
    else if (sg_type == "int")
    {
        teaInt_t::iterator nameIndex{std::find_if(teaInts.begin(), teaInts.end(),
                                                  [&](const TeaInt &ti) noexcept -> const bool {
                                                      return ti.getname() == sg_name;
                                                  })};
        if (nameIndex == teaInts.end())
            teaSyntaxError(line, filename, "Variable name not found.");
        teaInts.erase(nameIndex);
    }
    else if (sg_type == "string")
    {
        teaString_t::iterator nameIndex{std::find_if(teaStrings.begin(), teaStrings.end(),
                                                     [&](const TeaString &ts) noexcept -> const bool {
                                                         return ts.getname() == sg_name;
                                                     })};
        if (nameIndex == teaStrings.end())
            teaSyntaxError(line, filename, "Variable name not found.");
        teaStrings.erase(nameIndex);
    }
    else
        teaSyntaxError(line, filename, "Invalid type specifier.");
}
