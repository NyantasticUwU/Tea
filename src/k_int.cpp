#include "createvar.hpp"
#include "error.hpp"
#include "k_int.hpp"

// Defining static globals (hence the sg_ prefix)
// These are defined here for performance reasons
static unsigned int sg_offPos;
static std::string sg_substr;

// Declaring external globals for defining variables in tea
extern int g_secondSpaceIndex;
extern std::string g_varname;

// Called when the int keyword is called in tea
void kInt(const std::string &statement, const int &line, const char *&filename, teaInt_t &teaInts)
{
    createvar(statement, line, filename, 3);
    sg_substr = statement.substr(g_secondSpaceIndex + 1, statement.size());
    teaInts.push_back(TeaInt{g_varname, std::stoi(sg_substr, &sg_offPos, 0)});
    if (sg_offPos != sg_substr.size())
        teaSyntaxError(line, filename, "Invalid integral number.");
}
