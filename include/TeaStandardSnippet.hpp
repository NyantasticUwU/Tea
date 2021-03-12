#ifndef TEA_TEA_STANDARD_SNIPPET_HPP_INCLUDED
#define TEA_TEA_STANDARD_SNIPPET_HPP_INCLUDED
#include "TeaArray.hpp"
#include "TeaFloat.hpp"
#include "TeaInt.hpp"
#include "TeaSnippet.hpp"
#include "TeaString.hpp"
#include <functional>

// Using declarations
using standardsnippet_t = std::function<void(tea::teaString_t &teaStrings, tea::teaInt_t &teaInts,
    tea::teaFloat_t &teaFloats, tea::teaSnippet_t &teaSnippets, tea::teaArray_t &teaArrays)>;

// Represents a snippet within teas std lib
struct TeaStandardSnippet final
{
    const char *name;
    const standardsnippet_t snippet;
};

#endif
