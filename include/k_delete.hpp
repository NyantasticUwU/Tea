#ifndef TEA_K_DELETE_HPP_INCLUDED
#define TEA_K_DELETE_HPP_INCLUDED
#include "TeaFloat.hpp"
#include "TeaInt.hpp"
#include "TeaSnippet.hpp"
#include "TeaString.hpp"

// Called when the delete keyword is called in tea
void kDelete(
    const std::string &statement, const int &line, const char *&filename, teaString_t &teaStrings,
    teaInt_t &teaInts, teaFloat_t &teaFloats, teaSnippet_t &teaSnippets);

#endif
