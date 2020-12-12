#include "cmd.hpp"
#include "error.hpp"

// Extracts the file name of the .tea file from the argument vector
const char *&getMainFileName(const int &argc, const char **&argv)
{
    // Making sure a file name was given in the argument vector
    if (argc < 2)
        teaError("ERROR: File name was not supplied in the argument vector.\n"
                 "Exiting with code 1639 (ERROR_INVALID_COMMAND_LINE).",
                 1639); // Windows' ERROR_INVALID_COMMAND_LINE (1639)
    return argv[1];
}

// Fills tea string vector with cmd args
void fillTeaStringVec(const int &argc, const char **&argv, teaString_t &teaStrings)
{
    for (int i{0}; i < argc; ++i)
        teaStrings.push_back(TeaString{"arg" + std::to_string(i), argv[i]});
}
