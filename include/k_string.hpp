#ifndef TEA_K_STRING_HPP_INCLUDED
#define TEA_K_STRING_HPP_INCLUDED
#include "TeaString.hpp"

// Called when string keyword is called in tea
void kString(const std::string &statement, const int &line, const char *&filename, tea::teaString_t &teaStrings,
    const std::string &currentNamespace);

#endif
