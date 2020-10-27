#include "cmd.hpp"
#include "fileIO.hpp"
#include "parsefile.hpp"

// Main entry point of program
int main(const int argc, const char **argv)
{
    const char *&filename{getMainFileName(argc, argv)};
    checkMainFile(filename);
    std::vector<std::string> mainfile;
    parseFile(filename, mainfile);
    return 0;
}
