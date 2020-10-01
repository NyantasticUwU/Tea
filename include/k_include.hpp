#ifndef TEA_K_INCLUDE_HPP_INCLUDED
#define TEA_K_INCLUDE_HPP_INCLUDED
#include <string>
#include <vector>

// This function is called whenever the include keyword is used
void kInclude(std::string statement, std::vector<std::string> &parsedFile, const short &index);

#endif
