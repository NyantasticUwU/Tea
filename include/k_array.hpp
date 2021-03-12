#ifndef TEA_K_ARRAY_HPP_INCLUDED
#define TEA_K_ARRAY_HPP_INCLUDED
#include "TeaArray.hpp"

// Called when the array keyword is called in tea
void kArray(const std::string &statement, const int &line, const char *&filename, tea::teaArray_t &teaArrays,
    const std::string &currentNamespace);

#endif
