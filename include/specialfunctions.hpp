#ifndef TEA_SPECIAL_FUNCTIONS_HPP_INCLUDED
#define TEA_SPECIAL_FUNCTIONS_HPP_INCLUDED
#include <string>

// Holds all special tea functions
namespace special_functions
{
    // Called when a special function is called in tea
    void specialFunctionCalled(std::string &statement, const int &funcIndex);

    // Called when exit is called in tea
    void sf_exit(std::string &statement);

    // Called when faout is called in tea
    void sf_faout(std::string &statement);

    // Called when fout is called in tea
    void sf_fout(std::string &statement);

    // Called when pause is called in tea
    void sf_pause(std::string &statement);

    // Called when print is called in tea
    void sf_print(std::string &statement);

    // Called when printline is called in tea
    void sf_printline(std::string &statement);

    // Called when remove_file is called in tea
    void sf_remove_file(std::string &statement);
} // namespace special_functions

#endif
