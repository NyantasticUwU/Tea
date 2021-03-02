#ifndef TEA_K_CALL_HPP_INCLUDED
#define TEA_K_CALL_HPP_INCLUDED
#include "TeaArray.hpp"
#include "TeaFloat.hpp"
#include "TeaInt.hpp"
#include "TeaSnippet.hpp"
#include "TeaString.hpp"

// Called when the call keyword is called in tea
void kCall(const std::string &statement, const int &line, const char *&filename, teaString_t &teaStrings,
    teaInt_t &teaInts, teaFloat_t &teaFloats, teaSnippet_t &teaSnippets, teaArray_t &teaArrays,
    const std::string &currentNamespace);

#endif
