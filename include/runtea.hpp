#ifndef TEA_RUN_TEA_HPP_INCLUDED
#define TEA_RUN_TEA_HPP_INCLUDED
#include "TeaFloat.hpp"
#include "TeaInt.hpp"
#include "TeaString.hpp"

// Runs tea string vector
void runTea(std::vector<std::string> &teafile, const char *&filename, const teaFloat_t *pteaFloats = nullptr,
            const teaInt_t *pteaInts = nullptr, const teaString_t *pteaStrings = nullptr);

#endif
