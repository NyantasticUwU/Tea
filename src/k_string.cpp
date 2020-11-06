#include "createvar.hpp"
#include "k_string.hpp"
#include "kwstr.hpp"

// Defining static globals (hence the sg_ prefix)
// These are defined here for performance reasons
static int sg_secondSpaceIndex;
static std::string sg_varname;
static std::string sg_varval;

// Called when string keyword is called in tea
void kString(const std::string &statement, const int &line, const char *&filename, teaString_t &teaStrings)
{
    createvar(statement, line, filename, 6, sg_varname, sg_secondSpaceIndex);
    sg_varval = getStringLiteral(
        statement.substr(0, 6).append(statement.substr(sg_secondSpaceIndex, statement.size())), line, filename, 6);
    teaStrings.push_back(TeaString{sg_varname, sg_varval});
}
