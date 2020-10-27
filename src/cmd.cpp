#include "cmd.hpp"
#include <iostream>

// Extracts the file name of the .tea file from the argument vector
const char *&getMainFileName(const int &argc, const char **&argv)
{
    // Making sure a file name was given in the argument vector
    if (argc < 2)
    {
        std::cerr << "ERROR: File name was not supplied in the argument vector.\n"
                     "Exiting with code 1639 (ERROR_INVALID_COMMAND_LINE).";
        std::exit(1639); // Windows' ERROR_INVALID_COMMAND_LINE (1639)
    }
    return argv[1];
}
