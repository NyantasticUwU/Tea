#include "cmd.hpp"
#include "fileIO.hpp"
#include "parsefile.hpp"
#include "runtea.hpp"

// Defining globals
std::vector<std::string> g_teakeywords{
    "delete",
    "emplace",
    "float",
    "include",
    "int",
    "string",
    "system"};

// Main entry point of program
int main(const int argc, const char **argv)
{
    const char *&filename{getMainFileName(argc, argv)};
    checkFile(filename);
    std::vector<std::string> mainfile;
    parseFile(filename, mainfile);
    runTea(mainfile, filename);
    return 0;
}
