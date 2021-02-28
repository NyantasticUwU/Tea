#ifndef TEA_K_IF_HPP_INCLUDED
#define TEA_K_IF_HPP_INCLUDED
#include "TeaArray.hpp"
#include "TeaFloat.hpp"
#include "TeaSnippet.hpp"
#include "TeaInt.hpp"
#include "TeaString.hpp"

// Checks if tea if statement evaluates to true
const bool isIfTrue(const std::string &statement, const int &line, const char *&filename, const int &kwlen);

// Checks if statement enters a new block
const bool isEnteringBlock(const std::string &statement) noexcept;

// Called when if keyword is called in tea
void kIf(const std::vector<std::string> &teafile, const int &teafileSize, const std::string &statement, int &line,
    const char *&filename, teaString_t &teaStrings, teaInt_t &teaInts, teaFloat_t &teaFloats,
    teaSnippet_t &teaSnippets, teaArray_t &teaArrays);

#endif
