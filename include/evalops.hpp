#ifndef TEA_EVAL_OPS_HPP_INCLUDED
#define TEA_EVAL_OPS_HPP_INCLUDED
#include <string>

// Evaluates and operators in a given string
// Returns modified string with value(s) in-place
void evalOps(std::string &statement, const int &line, const char *&filename);

#endif
