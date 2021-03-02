#ifndef TEA_K_INT_HPP_INCLUDED
#define TEA_K_INT_HPP_INCLUDED
#include "TeaInt.hpp"

// Called when the int keyword is called in tea
void kInt(const std::string &statement, const int &line, const char *&filename, teaInt_t &teaInts,
    const std::string &currentNamespace);

#endif
