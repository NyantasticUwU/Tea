#include "error.hpp"
#include "k_delete.hpp"
#include <algorithm>

// Defining static globals (hence the sg_ prefix)
// These are defined here for performance reasons
static std::size_t sg_i;
static std::size_t sg_statementSize;
static std::string sg_type;

// Gets variable type
static void getType(const std::string &statement)
{
    sg_type.clear();
    for (sg_i = 7U; sg_i < sg_statementSize; ++sg_i)
    {
        if (statement[sg_i] == ' ')
        {
            ++sg_i;
            break;
        }
        sg_type.push_back(statement[sg_i]);
    }
}

// Called when the delete keyword is called in tea
void kDelete(const std::string &statement, const int &line, const char *&filename, teaString_t &teaStrings,
             teaInt_t &teaInts, teaFloat_t &teaFloats)
{
    if (std::count(statement.begin(), statement.end(), ' ') != 2)
        teaSyntaxError(line, filename);
    sg_statementSize = statement.size();
    getType(statement);
    if (sg_type == "string")
    {
        static teaString_t::iterator nameIndex;
        nameIndex = std::find_if(teaStrings.begin(), teaStrings.end(),
                                 [&](const TeaString &ts) -> const bool {
                                     return ts.getname() == statement.substr(sg_i, sg_statementSize);
                                 });
        if (nameIndex == teaStrings.end())
            teaSyntaxError(line, filename, "Variable name not found.");
        teaStrings.erase(nameIndex);
    }
    else if (sg_type == "int")
    {
        static teaInt_t::iterator nameIndex;
        nameIndex = std::find_if(teaInts.begin(), teaInts.end(),
                                 [&](const TeaInt &ti) -> const bool {
                                     return ti.getname() == statement.substr(sg_i, sg_statementSize);
                                 });
        if (nameIndex == teaInts.end())
            teaSyntaxError(line, filename, "Variable name not found.");
        teaInts.erase(nameIndex);
    }
    else if (sg_type == "float")
    {
        static teaFloat_t::iterator nameIndex;
        nameIndex = std::find_if(teaFloats.begin(), teaFloats.end(),
                                 [&](const TeaFloat &tf) -> const bool {
                                     return tf.getname() == statement.substr(sg_i, sg_statementSize);
                                 });
        if (nameIndex == teaFloats.end())
            teaSyntaxError(line, filename, "Variable name not found.");
        teaFloats.erase(nameIndex);
    }
    else
        teaSyntaxError(line, filename, "Invalid type specifier.");
}
