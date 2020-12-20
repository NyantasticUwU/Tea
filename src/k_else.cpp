#include "k_elif.hpp"
#include "k_else.hpp"

// Called when the elif keyword is called in tea
const bool kElse(int &line, const std::vector<std::string> &teafile, const int &teafileSize)
{
    return kElif(line, teafile, teafileSize);
}
