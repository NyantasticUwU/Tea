#include "error.hpp"
#include "k_delete.hpp"
#include <algorithm>

// Defining static globals (hence the sg_ prefix)
// These are defined here for performance reasons
static thread_local std::size_t sg_i;
static thread_local std::string sg_type;

// Gets variable type
static inline void getType(const std::string &statement, const std::size_t &statementSize)
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

// Removes variable from tea type vector if name matches
template <typename T>
static inline void deleteVariableByName(const std::string &substr, std::vector<T> &typevec, const int &line,
    const char *&filename)
{
    using TeaTypeVectorIterator = typename std::vector<T>::iterator;
    const TeaTypeVectorIterator &&begin{typevec.begin() - 1};
    for (TeaTypeVectorIterator &&i{typevec.end() - 1}; i != begin; --i)
    {
        if (i->getname() == substr)
        {
            typevec.erase(i);
            return;
        }
    }
    teaSyntaxError(line, filename, "Variable name not found.");
}

// Called when the delete keyword is called in tea
void kDelete(const std::string &statement, const int &line, const char *&filename, tea::teaString_t &teaStrings,
    tea::teaInt_t &teaInts, tea::teaFloat_t &teaFloats, tea::teaSnippet_t &teaSnippets, tea::teaArray_t &teaArrays)
{
    if (std::count(statement.begin(), statement.end(), ' ') != 2)
        teaSyntaxError(line, filename);
    const std::size_t &&statementSize{statement.size()};
    getType(statement, statementSize);
    const std::string &&substr{statement.substr(sg_i)};
    if (sg_type == "string")
        deleteVariableByName(substr, teaStrings, line, filename);
    else if (sg_type == "int")
        deleteVariableByName(substr, teaInts, line, filename);
    else if (sg_type == "float")
        deleteVariableByName(substr, teaFloats, line, filename);
    else if (sg_type == "snippet")
        deleteVariableByName(substr, teaSnippets, line, filename);
    else if (sg_type == "array")
        deleteVariableByName(substr, teaArrays, line, filename);
    else
        teaSyntaxError(line, filename, "Invalid type specifier.");
}
