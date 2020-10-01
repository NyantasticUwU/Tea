#include "cmd.hpp"
#include "parsefile.hpp"
#include "runtea.hpp"

// Main entry point of program
int main(int argc, char **argv)
{
    char *&filename{getFilename(argc, argv)};
    std::vector<std::string> parsedfile{parseFile(std::string{filename})};
    runTea(parsedfile);
    return 0;
}
