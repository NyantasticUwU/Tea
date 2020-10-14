#ifndef TEA_K_FUNCTION_HPP_INCLUDED
#define TEA_K_FUNCTION_HPP_INCLUDED
#include "constants.hpp"
#include <string>
#include <vector>

// Defining function type
using function_t = std::vector<std::pair<std::string, std::vector<std::string>>>;

// This function is called when the function keyword is used
void kFunction(std::string &statement, function_t &t_functions, function_t &t_functionParams,
               const std::vector<std::string> &parsedFile, int16_dynamic_t &index);

// This function is called when a tea function is encountered
void functionCalled(const std::string funcName, std::string &statement, std::vector<std::string> &functionBody,
                    const function_t &t_functionParams, std::vector<std::string> &parsedFile,
                    const int16_dynamic_t &index);

#endif
