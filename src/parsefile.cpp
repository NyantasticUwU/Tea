#include "error.hpp"
#include "parsefile.hpp"
#include <fstream>

// Strips line of blank space
static void stripLine(std::string &line)
{
    static std::size_t s_i;
    static std::size_t s_lineSize;
    // Stripping front of line
    for (s_i = 0U; s_i < line.size(); ++s_i)
    {
        if (!std::isblank(line[s_i]))
            break;
        line.erase(0U, 1U);
        --s_i; // If s_i underflows, the next iteration of the loop will fix it
    }
    // Stripping end of line
    s_lineSize = line.size();
    while (s_lineSize > 0U)
    {
        if (!std::isblank(line[s_lineSize - 1U]))
            break;
        line.pop_back();
        s_lineSize = line.size();
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
