#ifndef TEA_RUN_TEA_HPP_INCLUDED
#define TEA_RUN_TEA_HPP_INCLUDED
#include "TeaFloat.hpp"
#include "TeaInt.hpp"
#include "TeaString.hpp"

// Runs tea string vector
void runTea(std::vector<std::string> &teafile, const char *&filename,
            const teaString_t *const &pteaStrings = nullptr, const teaInt_t *const &pteaInts = nullptr,
            const teaFloat_t *const &pteaFloats = nullptr);

// Runs tea statement
void loopTeaStatements(std::vector<std::string> &teafile, int &line, const char *&filename,
                       teaString_t &teaStrings, teaInt_t &teaInts, teaFloat_t &teaFloats);

#endif
