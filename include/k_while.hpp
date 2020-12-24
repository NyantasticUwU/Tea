#ifndef TEA_K_WHILE_HPP_INCLUDED
#define TEA_K_WHILE_HPP_INCLUDED
#include "TeaFloat.hpp"
#include "TeaInt.hpp"
#include "TeaString.hpp"

// Called when while keyword is called in tea
void kWhile(std::vector<std::string> &teafile, const int &teafileSize, const std::string &statement,
            const std::string unchangedStatement, int &line, const char *&filename, teaString_t &teaStrings,
            teaInt_t &teaInts, teaFloat_t &teaFloats);

#endif
