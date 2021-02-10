#ifndef TEA_K_EMPLACE_HPP_INCLUDED
#define TEA_K_EMPLACE_HPP_INCLUDED
#include "TeaArray.hpp"
#include "TeaFloat.hpp"
#include "TeaInt.hpp"
#include "TeaString.hpp"

// Emplaces variables into statement
void kEmplace(std::string &prestatement, const int &line, const char *&filename, const teaString_t &teaStrings,
    const teaInt_t &teaInts, const teaFloat_t &teaFloats, const teaArray_t &teaArrays);

#endif
