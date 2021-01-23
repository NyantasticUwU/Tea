#ifndef TEA_TEA_STANDARD_SNIPPET_HPP_INCLUDED
#define TEA_TEA_STANDARD_SNIPPET_HPP_INCLUDED
#include "TeaFloat.hpp"
#include "TeaInt.hpp"
#include "TeaSnippet.hpp"
#include "TeaString.hpp"
#include <functional>

// Using declarations
using standardsnippet_t = std::function<void(teaString_t &teaStrings, teaInt_t &teaInts, teaFloat_t &teaFloats,
                                             teaSnippet_t &teaSnippets)>;

// Represents a snippet within teas std lib
struct TeaStandardSnippet
{
    const char *name;
    const standardsnippet_t snippet;
};

#endif
