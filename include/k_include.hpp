#ifndef TEA_K_INCLUDE_HPP_INCLUDED
#define TEA_K_INCLUDE_HPP_INCLUDED
#include "constants.hpp"
#include <string>
#include <vector>

// This function is called whenever the include keyword is used
void kInclude(std::string &statement, std::vector<std::string> &parsedFile, const int16_dynamic_t &index);

#endif
