#include "k_elif.hpp"

// Called when the elif keyword is called in tea
const bool kElif(int &line, const std::vector<std::string> &teafile, const int &teafileSize)
{
    while (line < teafileSize)
    {
        if (teafile[line++] == "end")
            return true;
    }
    return false;
}
