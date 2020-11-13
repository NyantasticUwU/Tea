#ifndef TEA_K_EMPLACE_HPP_INCLUDED
#define TEA_K_EMPLACE_HPP_INCLUDED
#include "TeaFloat.hpp"
#include "TeaInt.hpp"
#include "TeaString.hpp"

// Emplaces variables into statement
void kEmplace(std::string &prestatement, const teaString_t &teaStrings, const teaInt_t &teaInts,
              const teaFloat_t &teaFloats);

#endif
