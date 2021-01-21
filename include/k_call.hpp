#ifndef TEA_K_CALL_HPP_INCLUDED
#define TEA_K_CALL_HPP_INCLUDED
#include "TeaFloat.hpp"
#include "TeaInt.hpp"
#include "TeaSnippet.hpp"
#include "TeaString.hpp"

// Called when the call keyword is called in tea
void kCall(const std::string &statement, const int &line, const char *&filename, const teaString_t &teaStrings,
           const teaInt_t &teaInts, const teaFloat_t &teaFloats, const teaSnippet_t &teaSnippets);

#endif
