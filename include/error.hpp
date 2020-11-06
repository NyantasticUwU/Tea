#ifndef TEA_ERROR_HPP_INCLUDED
#define TEA_ERROR_HPP_INCLUDED
#include <string>

// Outputs error message and exits program
void teaError(const std::string &msg, const int &exitCode);

// Throws ERROR_INVALID_DATA error with custom message
void teaSyntaxError(const int &line, const char *&filename, const std::string &msg = "");

#endif
