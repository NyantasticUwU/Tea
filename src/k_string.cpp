#include "createvar.hpp"
#include "k_string.hpp"
#include "kwstr.hpp"

// Defining static globals (hence the sg_ prefix)
// These are defined here for performance reasons
static int sg_secondSpaceIndex;
static std::string sg_varname;
static std::string sg_varval;

// Declaring tea value vectors
extern teaString_t g_teaStrings;

// Called when string keyword is called in tea
void kString(const std::string &statement, const int &line)
{
    createvar(statement, line, 6, sg_varname, sg_secondSpaceIndex);
    sg_varval = getStringLiteral(
        statement.substr(0, 6).append(statement.substr(sg_secondSpaceIndex, statement.size())), line, 6);
    g_teaStrings.push_back(TeaString{sg_varname, sg_varval});
}
