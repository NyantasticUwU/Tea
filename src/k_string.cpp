#include "createvar.hpp"
#include "k_string.hpp"
#include "kwstr.hpp"

// Defining static globals (hence the sg_ prefix)
// These are defined here for performance reasons
static std::string sg_varval;

// Declaring external globals for defining variables in tea
extern int g_secondSpaceIndex;
extern std::string g_varname;

// Called when string keyword is called in tea
void kString(const std::string &statement, const int &line, const char *&filename, teaString_t &teaStrings)
{
    createvar(statement, line, filename, 6);
    sg_varval = getStringLiteral(
        statement.substr(0, 6).append(statement.substr(g_secondSpaceIndex, statement.size())), line, filename, 6);
    teaStrings.push_back(TeaString{g_varname, sg_varval});
}
