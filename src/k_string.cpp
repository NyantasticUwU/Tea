#include "createvar.hpp"
#include "k_string.hpp"
#include "kwstr.hpp"

// Declaring external globals for defining variables in tea
extern thread_local int g_secondSpaceIndex;
extern thread_local std::string g_varname;

// Called when string keyword is called in tea
void kString(const std::string &statement, const int &line, const char *&filename, tea::teaString_t &teaStrings,
    const std::string &currentNamespace)
{
    createvar(statement, line, filename, 6);
    teaStrings.push_back(tea::TeaString{
        currentNamespace + g_varname,
        getStringLiteral(
            statement.substr(0U, 6U).append(statement.substr(g_secondSpaceIndex, statement.size())),
            line, filename, 6)});
}
