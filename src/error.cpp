#include "error.hpp"
#include <iostream>

// Outputs error message and exits program
void teaError(const std::string &msg, const int &exitCode)
{
    std::cerr << msg;
    std::exit(exitCode);
}

// Throws ERROR_INVALID_DATA error with custom message
void teaSyntaxError(const int &line, const std::string &msg)
{
    teaError("ERROR: Invalid statement on line " + std::to_string(line) + ".\n" +
                 msg + (msg == "" ? "" : "\n") +
                 "Exiting with code 13 (ERROR_INVALID_DATA).",
             13); // Windows' ERROR_INVALID_DATA (13)
}
