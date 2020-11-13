#include "createvar.hpp"
#include "error.hpp"
#include "k_float.hpp"

// Defining static globals (hence the sg_ prefix)
// These are defined here for performance reasons
static unsigned int sg_offPos;
static std::string sg_substr;

// Declaring external globals for defining variables in tea
extern int g_secondSpaceIndex;
extern std::string g_varname;

// Called when the float keyword is called in tea
void kFloat(const std::string &statement, const int &line, const char *&filename, teaFloat_t &teaFloats)
{
    createvar(statement, line, filename, 5);
    sg_substr = statement.substr(g_secondSpaceIndex + 1, statement.size());
    teaFloats.push_back(TeaFloat{g_varname, std::stof(sg_substr, &sg_offPos)});
    if (sg_offPos != sg_substr.size())
        teaSyntaxError(line, filename, "Invalid float literal.");
}
