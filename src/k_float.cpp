#include "createvar.hpp"
#include "error.hpp"
#include "k_float.hpp"

// Declaring external globals for defining variables in tea
extern int g_secondSpaceIndex;
extern std::string g_varname;

// Called when the float keyword is called in tea
void kFloat(const std::string &statement, const int &line, const char *&filename, teaFloat_t &teaFloats,
    const std::string &currentNamespace)
{
    static std::size_t s_offPos;
    createvar(statement, line, filename, 5);
    const std::string &&substr{statement.substr(g_secondSpaceIndex + 1, statement.size())};
    teaFloats.push_back(TeaFloat{currentNamespace + g_varname, std::stof(substr, &s_offPos)});
    if (s_offPos != substr.size())
        teaSyntaxError(line, filename, "Invalid float literal.");
}
