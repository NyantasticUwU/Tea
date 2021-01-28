#ifndef TEA_KWSTR_HPP_INCLUDED
#define TEA_KWSTR_HPP_INCLUDED
#include <string>

// Extracts chars inside string literal and puts it in command
std::string &getStringLiteral(
    const std::string &statement, const int &line, const char *&filename, const int &kwlen);

#endif
