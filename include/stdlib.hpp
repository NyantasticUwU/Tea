#ifndef TEA_STD_LIB_HPP_INCLUDED
#define TEA_STD_LIB_HPP_INCLUDED
#include "TeaFloat.hpp"
#include "TeaInt.hpp"
#include "TeaSnippet.hpp"
#include "TeaString.hpp"

// Handles snippet by given name
const bool handleStandardSnippet(const std::string &snippetName, const int &line, const char *&filename,
    teaString_t &teaStrings, teaInt_t &teaInts, teaFloat_t &teaFloats, teaSnippet_t &teaSnippets,
    teaArray_t &teaArrays, const std::string &currentNamespace);

#endif
