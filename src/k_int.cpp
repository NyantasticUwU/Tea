#include "createvar.hpp"
#include "error.hpp"
#include "k_int.hpp"

// Declaring external globals for defining variables in tea
extern int g_secondSpaceIndex;
extern std::string g_varname;

// Called when the int keyword is called in tea
void kInt(const std::string &statement, const int &line, const char *&filename, teaInt_t &teaInts,
    const std::string &currentNamespace)
{
    static std::size_t s_offPos;
    createvar(statement, line, filename, 3);
    const std::string &&substr{statement.substr(g_secondSpaceIndex + 1, statement.size())};
    teaInts.push_back(TeaInt{currentNamespace + g_varname, std::stoi(substr, &s_offPos, 0)});
    if (s_offPos != substr.size())
        teaSyntaxError(line, filename, "Invalid integral number.");
}
