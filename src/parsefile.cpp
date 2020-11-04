#include "error.hpp"
#include "parsefile.hpp"
#include <fstream>

// Defining static globals
// These are defined here for performance reasons
static std::ifstream sg_f;
static std::string sg_line;

// Parses given file (c-str) into a string vector containing each sg_line in the file
void parseFile(const char *&filename, std::vector<std::string> &file)
{
    // Creating and opening input file stream
    sg_f.open(filename);
    if (!sg_f)
    {
        sg_f.close();
        teaError("ERROR: The given file was unable to be opened.\n"
                 "Exiting with code 110 (ERROR_OPEN_FAILED).",
                 110); // Windows' ERROR_OPEN_FAILED (110)
    }
    // Reading data from file into the file string vector (sg_line by sg_line)
    while (sg_f)
    {
        std::getline(sg_f, sg_line);
        file.push_back(sg_line);
    }
    sg_f.close();
}
