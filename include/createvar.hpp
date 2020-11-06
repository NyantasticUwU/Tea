#ifndef TEA_CREATE_VAR_HPP_INCLUDED
#define TEA_CREATE_VAR_HPP_INCLUDED
#include <string>

// Creates var
void createvar(const std::string &statement, const int &line, const char *&filename, const int &kwlen,
               std::string &varname, int &secondSpaceIndex);

#endif
