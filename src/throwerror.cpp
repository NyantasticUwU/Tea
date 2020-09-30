#include "throwerror.hpp"
#include <iostream>

// This function is called when an error needs to be thrown
void throwError(const std::string &statement)
{
    std::cout << "Invalid statement: " << statement;
    std::exit(1);
}
