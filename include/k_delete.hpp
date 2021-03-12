#ifndef TEA_K_DELETE_HPP_INCLUDED
#define TEA_K_DELETE_HPP_INCLUDED
#include "TeaArray.hpp"
#include "TeaFloat.hpp"
#include "TeaInt.hpp"
#include "TeaSnippet.hpp"
#include "TeaString.hpp"

// Called when the delete keyword is called in tea
void kDelete(const std::string &statement, const int &line, const char *&filename, tea::teaString_t &teaStrings,
    tea::teaInt_t &teaInts, tea::teaFloat_t &teaFloats, tea::teaSnippet_t &teaSnippets,
    tea::teaArray_t &teaArrays);

#endif
