#ifndef TEA_K_IMPORT_HPP_INCLUDED
#define TEA_K_IMPORT_HPP_INCLUDED
#include "TeaArray.hpp"
#include "TeaFloat.hpp"
#include "TeaInt.hpp"
#include "TeaSnippet.hpp"
#include "TeaString.hpp"

// Called when import keyword is called in tea
void kImport(const std::string &statement, const int &line, const char *&filename, const teaString_t &teaStrings,
    const teaInt_t &teaInts, const teaFloat_t &teaFloats, const teaSnippet_t &teaSnippets,
    const teaArray_t &teaArrays);

#endif
