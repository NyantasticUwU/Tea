#include "createvar.hpp"
#include "error.hpp"
#include "k_float.hpp"

// Declaring external globals for defining variables in tea
extern int g_secondSpaceIndex;
extern std::string g_varname;

// Called when the float keyword is called in tea
void kFloat(const std::string &statement, const int &line, const char *&filename, teaFloat_t &teaFloats)
{
    static std::size_t s_offPos;
    static std::string s_substr;
    createvar(statement, line, filename, 5);
    s_substr = statement.substr(g_secondSpaceIndex + 1, statement.size());
    teaFloats.push_back(TeaFloat{g_varname, std::stof(s_substr, &s_offPos)});
    if (s_offPos != s_substr.size())
        teaSyntaxError(line, filename, "Invalid float literal.");
}
