#ifndef TEA_K_FLOAT_HPP_INCLUDED
#define TEA_K_FLOAT_HPP_INCLUDED
#include "TeaFloat.hpp"

// Called when the float keyword is called in tea
void kFloat(const std::string &statement, const int &line, const char *&filename, teaFloat_t &teaFloats,
    const std::string &currentNamespace);

#endif
