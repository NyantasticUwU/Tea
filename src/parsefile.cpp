#include "error.hpp"
#include "parsefile.hpp"
#include <fstream>

// Defining static globals
// These are defined here for performance reasons
static std::ifstream g_f;
static std::string g_line;

// Parses given file (c-str) into a string vector containing each g_line in the file
void parseFile(const char *&filename, std::vector<std::string> &file)
{
    // Creating and opening input file stream
    g_f.open(filename);
    if (!g_f)
    {
        g_f.close();
        teaError("ERROR: The given file was unable to be opened.\n"
                 "Exiting with code 110 (ERROR_OPEN_FAILED).",
                 110); // Windows' ERROR_OPEN_FAILED (110)
    }
    // Reading data from file into the file string vector (g_line by g_line)
    while (g_f)
    {
        std::getline(g_f, g_line);
        file.push_back(g_line);
    }
    g_f.close();
}
