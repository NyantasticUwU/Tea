#include "error.hpp"
#include "parsefile.hpp"
#include <fstream>

// Strips line of blank space
static void stripLine(std::string &line)
{
    // Stripping front of line
    while (line.size() > 0U)
    {
        if (!std::isblank(line.front()))
            break;
        line.erase(0U, 1U);
    }
    // Stripping end of line
    while (line.size() > 0U)
    {
        if (!std::isblank(line.back()))
            break;
        line.pop_back();
    }
}

// Parses given file (c-str) into a string vector containing each line in the file
void parseFile(const char *&filename, std::vector<std::string> &file)
{
    static std::ifstream s_f;
    static std::string s_line;
    // Creating and opening input file stream
    s_f.open(filename);
    if (!s_f)
    {
        s_f.close();
        teaError("ERROR: The given file was unable to be opened.\n"
                 "Exiting with code 110 (ERROR_OPEN_FAILED).",
                 110); // Windows' ERROR_OPEN_FAILED (110)
    }
    // Reading data from file into the file string vector (line by line)
    while (std::getline(s_f, s_line))
    {
        stripLine(s_line);
        file.push_back(s_line);
    }
    s_line.clear();
    s_f.close();
}
