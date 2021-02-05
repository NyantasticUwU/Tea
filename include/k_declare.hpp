#ifndef TEA_K_DECLARE_HPP_INCLUDED
#define TEA_K_DECLARE_HPP_INCLUDED
#include <string>

// Called when the declare keyword is called in tea
void kDeclare(const std::string &statement, const int &line, const char *&filename);

#endif
