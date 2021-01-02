#include "createvar.hpp"
#include "k_string.hpp"
#include "kwstr.hpp"

// Declaring external globals for defining variables in tea
extern int g_secondSpaceIndex;
extern std::string g_varname;

// Called when string keyword is called in tea
void kString(const std::string &statement, const int &line, const char *&filename, teaString_t &teaStrings)
{
    static std::string s_varval;
    createvar(statement, line, filename, 6);
    s_varval = getStringLiteral(
        statement.substr(0U, 6U).append(
            statement.substr(g_secondSpaceIndex, statement.size())),
        line, filename, 6);
    teaStrings.push_back(TeaString{g_varname, s_varval});
}
