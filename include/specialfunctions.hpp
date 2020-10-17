#ifndef TEA_SPECIAL_FUNCTIONS_HPP_INCLUDED
#define TEA_SPECIAL_FUNCTIONS_HPP_INCLUDED
#include <string>

// Holds all special tea functions
namespace special_functions
{
    // Called when a special function is called in tea
    void specialFunctionCalled(std::string &statement, const int &funcIndex);

    // Called when print is called in tea
    void sf_print(std::string &statement);
} // namespace special_functions

#endif
