#include "cmd.hpp"
#include "constants.hpp"
#include "fileIO.hpp"
#include "init.hpp"
#include "parsefile.hpp"
#include "runtea.hpp"

// Defining globals
const char *g_teakeywords[TEA_NUMBER_OF_KEYWORDS]{
    "assign",
    "delete",
    "elif",
    "else",
    "emplace",
    "end",
    "exit",
    "float",
    "if",
    "import",
    "include",
    "int",
    "string",
    "system",
    "while"};

// Main entry point of program
int main(const int argc, const char **argv)
{
    init();
    const char *&filename{getMainFileName(argc, argv)};
    checkFile(filename);
    std::vector<std::string> mainfile;
    parseFile(filename, mainfile);
    teaInt_t teaInts{TeaInt{"argc", argc}};
    teaString_t teaStrings;
    fillTeaStringVec(argc, argv, teaStrings);
    runTea(mainfile, filename, &teaStrings, &teaInts);
    return 0;
}
