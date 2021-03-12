#ifndef TEA_K_NAMESPACE_HPP_INCLUDED
#define TEA_K_NAMESPACE_HPP_INCLUDED
#include "TeaArray.hpp"
#include "TeaFloat.hpp"
#include "TeaInt.hpp"
#include "TeaSnippet.hpp"
#include "TeaString.hpp"

// Called when the namespace keyword is called in tea
void kNamespace(const std::vector<std::string> &teafile, const std::string &statement, int &line,
    const char *&filename, const std::string &currentNamespace, tea::teaString_t &teaStrings,
    tea::teaInt_t &teaInts, tea::teaFloat_t &teaFloats, tea::teaSnippet_t &teaSnippets,
    tea::teaArray_t &teaArrays);

#endif
