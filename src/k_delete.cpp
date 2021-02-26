#include "error.hpp"
#include "k_delete.hpp"
#include <algorithm>

// Defining static globals (hence the sg_ prefix)
// These are defined here for performance reasons
static std::size_t sg_i;
static std::string sg_type;

// Gets variable type
static void getType(const std::string &statement, const std::size_t &statementSize)
{
    sg_type.clear();
    for (sg_i = 7U; sg_i < statementSize; ++sg_i)
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
void kDelete(
    const std::string &statement, const int &line, const char *&filename, teaString_t &teaStrings,
    teaInt_t &teaInts, teaFloat_t &teaFloats, teaSnippet_t &teaSnippets, teaArray_t &teaArrays)
{
    if (std::count(statement.begin(), statement.end(), ' ') != 2)
        teaSyntaxError(line, filename);
    const std::size_t &&statementSize{statement.size()};
    getType(statement, statementSize);
    if (sg_type == "string")
    {
        const std::string &&substr{statement.substr(sg_i)};
        teaString_t::iterator &&s_nameIndex{std::find_if(
            teaStrings.begin(),
            teaStrings.end(),
            [&](const TeaString &ts) -> const bool {
                return ts.getname() == substr;
            })};
        if (s_nameIndex == teaStrings.end())
            teaSyntaxError(line, filename, "Variable name not found.");
        teaStrings.erase(s_nameIndex);
    }
    else if (sg_type == "int")
    {
        const std::string &&substr{statement.substr(sg_i)};
        teaInt_t::iterator &&s_nameIndex{std::find_if(
            teaInts.begin(),
            teaInts.end(),
            [&](const TeaInt &ti) -> const bool {
                return ti.getname() == substr;
            })};
        if (s_nameIndex == teaInts.end())
            teaSyntaxError(line, filename, "Variable name not found.");
        teaInts.erase(s_nameIndex);
    }
    else if (sg_type == "float")
    {
        const std::string &&substr{statement.substr(sg_i)};
        teaFloat_t::iterator &&s_nameIndex{std::find_if(
            teaFloats.begin(),
            teaFloats.end(),
            [&](const TeaFloat &tf) -> const bool {
                return tf.getname() == substr;
            })};
        if (s_nameIndex == teaFloats.end())
            teaSyntaxError(line, filename, "Variable name not found.");
        teaFloats.erase(s_nameIndex);
    }
    else if (sg_type == "snippet")
    {
        const std::string &&substr{statement.substr(sg_i)};
        teaSnippet_t::iterator &&s_nameIndex{std::find_if(
            teaSnippets.begin(),
            teaSnippets.end(),
            [&](const TeaSnippet &ts) -> const bool {
                return ts.getname() == substr;
            })};
        if (s_nameIndex == teaSnippets.end())
            teaSyntaxError(line, filename, "Snippet name not found.");
        teaSnippets.erase(s_nameIndex);
    }
    else if (sg_type == "array")
    {
        const std::string &&substr{statement.substr(sg_i)};
        teaArray_t::iterator &&s_nameIndex{std::find_if(
            teaArrays.begin(),
            teaArrays.end(),
            [&](const TeaArray<std::any> &ta) -> const bool {
                return ta.getname() == substr;
            })};
        if (s_nameIndex == teaArrays.end())
            teaSyntaxError(line, filename, "Array name not found.");
        teaArrays.erase(s_nameIndex);
    }
    else
        teaSyntaxError(line, filename, "Invalid type specifier.");
}
