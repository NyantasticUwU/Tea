#ifndef TEA_RUN_TEA_HPP_INCLUDED
#define TEA_RUN_TEA_HPP_INCLUDED
#include "TeaArray.hpp"
#include "TeaFloat.hpp"
#include "TeaSnippet.hpp"
#include "TeaInt.hpp"
#include "TeaString.hpp"

// Checks if statement starts with the given keyword
inline const bool startsWithKeyword(const std::string &statement, const char *const &teaKeyword) noexcept;

// Runs tea string vector
void runTea(const std::vector<std::string> &teafile, const char *&filename, const std::string &currentNamespace,
    const teaString_t *const &pteaStrings = nullptr, const teaInt_t *const &pteaInts = nullptr,
    const teaFloat_t *const &pteaFloats = nullptr, const teaSnippet_t *const &pteaSnippets = nullptr,
    const teaArray_t *const &pteaArrays = nullptr);

// Runs tea statement
void loopTeaStatements(const std::vector<std::string> &teafile, int &line, const char *&filename,
    teaString_t &teaStrings, teaInt_t &teaInts, teaFloat_t &teaFloats, teaSnippet_t &teaSnippets,
    teaArray_t &teaArrays, const std::string &currentNamespace);

#endif
