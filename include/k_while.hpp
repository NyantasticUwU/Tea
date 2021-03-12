#ifndef TEA_K_WHILE_HPP_INCLUDED
#define TEA_K_WHILE_HPP_INCLUDED
#include "TeaArray.hpp"
#include "TeaFloat.hpp"
#include "TeaSnippet.hpp"
#include "TeaInt.hpp"
#include "TeaString.hpp"

// Called when while keyword is called in tea
void kWhile(const std::vector<std::string> &teafile, const int &teafileSize, const std::string unchangedStatement,
    int &line, const char *&filename, tea::teaString_t &teaStrings, tea::teaInt_t &teaInts,
    tea::teaFloat_t &teaFloats, tea::teaSnippet_t &teaSnippets, tea::teaArray_t &teaArrays,
    const std::string &currentNamespace);

#endif
