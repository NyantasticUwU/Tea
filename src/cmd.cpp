#include "cmd.hpp"
#include <iostream>

// This function is used to extract the filename of the .tea file
char *&getFilename(const int &argc, char **&argv)
{
    if (argc <= 1)
    {
        std::cout << "ERROR: No file given!";
        std::exit(1);
    }
    return argv[1];
}
