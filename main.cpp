#include "cmd.hpp"
#include "fileIO.hpp"
#include "parsefile.hpp"
#include "runtea.hpp"

// Defining globals
std::vector<std::string> g_teakeywords{
    "delete",
    "emplace",
    "float",
    "import",
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
    teaInt_t teaInts{TeaInt{"argc", argc}};
    teaString_t teaStrings;
    fillTeaStringVec(argc, argv, teaStrings);
    runTea(mainfile, filename, &teaStrings, &teaInts, nullptr);
    return 0;
}
