#ifndef TEA_K_ASSIGN_HPP_INCLUDED
#define TEA_K_ASSIGN_HPP_INCLUDED
#include "TeaArray.hpp"
#include "TeaFloat.hpp"
#include "TeaInt.hpp"
#include "TeaString.hpp"

// Called when the assign keyword is called in tea
void kAssign(const std::string &statement, const int &line, const char *&filename, tea::teaString_t &teaStrings,
    tea::teaInt_t &teaInts, tea::teaFloat_t &teaFloats, tea::teaArray_t &teaArrays);

#endif
