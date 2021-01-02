#include "createvar.hpp"
#include "error.hpp"
#include "k_int.hpp"

// Declaring external globals for defining variables in tea
extern int g_secondSpaceIndex;
extern std::string g_varname;

// Called when the int keyword is called in tea
void kInt(const std::string &statement, const int &line, const char *&filename, teaInt_t &teaInts)
{
    static std::size_t s_offPos;
    static std::string s_substr;
    createvar(statement, line, filename, 3);
    s_substr = statement.substr(g_secondSpaceIndex + 1, statement.size());
    teaInts.push_back(TeaInt{g_varname, std::stoi(s_substr, &s_offPos, 0)});
    if (s_offPos != s_substr.size())
        teaSyntaxError(line, filename, "Invalid integral number.");
}
