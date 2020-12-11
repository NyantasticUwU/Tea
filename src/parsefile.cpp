#include "error.hpp"
#include "parsefile.hpp"
#include <fstream>

// Parses given file (c-str) into a string vector containing each line in the file
void parseFile(const char *&filename, std::vector<std::string> &file)
{
    // Creating and opening input file stream
    std::ifstream f{filename};
    if (!f)
    {
        f.close();
        teaError("ERROR: The given file was unable to be opened.\n"
                 "Exiting with code 110 (ERROR_OPEN_FAILED).",
                 110); // Windows' ERROR_OPEN_FAILED (110)
    }
    // Reading data from file into the file string vector (line by line)
    std::string line;
    while (std::getline(f, line))
        file.push_back(line);
    f.close();
}
