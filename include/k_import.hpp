#ifndef TEA_K_IMPORT_HPP_INCLUDED
#define TEA_K_IMPORT_HPP_INCLUDED
#include "TeaFloat.hpp"
#include "TeaInt.hpp"
#include "TeaString.hpp"

// Called when import keyword is called in tea
void kImport(const std::string &statement, const int &line, const char *&filename, const teaFloat_t &teaFloats,
             const teaInt_t &teaInts, const teaString_t &teaStrings);

#endif
