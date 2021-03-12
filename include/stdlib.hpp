#ifndef TEA_STD_LIB_HPP_INCLUDED
#define TEA_STD_LIB_HPP_INCLUDED
#include "TeaFloat.hpp"
#include "TeaInt.hpp"
#include "TeaSnippet.hpp"
#include "TeaString.hpp"

// Handles snippet by given name
const bool handleStandardSnippet(const std::string &snippetName, const int &line, const char *&filename,
    tea::teaString_t &teaStrings, tea::teaInt_t &teaInts, tea::teaFloat_t &teaFloats,
    tea::teaSnippet_t &teaSnippets, tea::teaArray_t &teaArrays, const std::string &currentNamespace);

#endif
