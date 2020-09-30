#include "cmd.hpp"
#include "parsefile.hpp"
#include <string>

// Main entry point of program
int main(int argc, char **argv)
{
    char *&filename{getFilename(argc, argv)};
    std::vector<std::string> parsedfile{parseFile(filename)};
    return 0;
}
