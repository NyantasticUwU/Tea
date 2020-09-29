#include "cmd.hpp"

// This function is used to extract the filename of the .tea file
char *&getFilename(const int &argc, char **&argv)
{
    return argv[1];
}
