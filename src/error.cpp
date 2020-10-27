#include "error.hpp"
#include <iostream>

// Outputs error message and exits program
void teaError(const std::string &msg, const int &exitCode)
{
    std::cout << msg;
    std::exit(exitCode);
}
